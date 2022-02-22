#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/types.h>

#include<linux/slab.h>

#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <linux/hash.h>
#include <linux/list.h>
#include <linux/vmalloc.h>
#include <linux/delay.h>
 
#include "ApollonKernel_driver.h"

#include <linux/ioport.h>
#include <linux/interrupt.h> //# Interrupt Implementation #
#include <asm/io.h>


#define IRQ 54 //# Interrupt Implementation #
 
#define FIRST_MINOR 0
#define MINOR_CNT 1

#define DEVICE_MEMORY_ENTRY_SIZE 65536

#define BASE 0x1c061000
#define ADDR_SIZE 0xfff

#define DMA_TRANSFER_ACTUATOR_TO_DEVICE   0x00000
#define DMA_TRANSFER_SENSOR_TO_DEVICE     0x00008
#define DMA_TRANSFER_SENSOR_FROM_DEVICE   0x00010
#define GEM_CURRENT_TICK                  0x00018

 
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
struct device *dev_ret;

static u8 ret;

static PtolemyActuator_t *   PtolemyActuator;
static PtolemySensorAttr_t * PtolemySensorAttr;
static PtolemySensorData_t * PtolemySensorData;

static void __iomem* ioremap_res; 

dma_addr_t bus_addr_actuator_to_device;
dma_addr_t bus_addr_sensor_to_device;
dma_addr_t bus_addr_sensor_from_device;


bool dma_from_device_completed;
 
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}


int ramdevice_init(void)
{
    PtolemySensorAttr = kmalloc(sizeof(PtolemySensorAttr_t),GFP_DMA);
    PtolemySensorData = kmalloc(sizeof(PtolemySensorData_t),GFP_DMA);
    PtolemyActuator   = kmalloc(sizeof(PtolemyActuator_t),GFP_DMA);
    
    
    ioremap_res = ioremap(BASE, ADDR_SIZE);
    
    
    /* --- DMA MAP SINGE FOR SENSOR DATA TO DEVICE --- */
    if(bus_addr_sensor_to_device != 0)
        dma_unmap_single(dev_ret, bus_addr_sensor_to_device, sizeof(PtolemySensorAttr_t), DMA_TO_DEVICE);
            
    bus_addr_sensor_to_device = dma_map_single(dev_ret, (PtolemySensorAttr_t *) PtolemySensorAttr, sizeof(PtolemySensorAttr_t), DMA_TO_DEVICE);
            
    if (dma_mapping_error(dev_ret, bus_addr_sensor_to_device)){
        printk(KERN_ALERT "\n\n\n---------- dma_MAP_ERROR: %pa\n\n\n\n",&bus_addr_sensor_to_device);
    }
    /* --- END DMA MAP SINGE FOR SENSOR DATA TO DEVICE --- */
    
    
    /* --- DMA MAP SINGE FOR SENSOR DATA FROM DEVICE --- */
    if(bus_addr_sensor_from_device != 0)
        dma_unmap_single(dev_ret, bus_addr_sensor_from_device, sizeof(PtolemySensorData_t), DMA_FROM_DEVICE);
            
    bus_addr_sensor_from_device = dma_map_single(dev_ret, (PtolemySensorData_t *) PtolemySensorData, sizeof(PtolemySensorData_t), DMA_FROM_DEVICE);
            
    if (dma_mapping_error(dev_ret, bus_addr_sensor_from_device)){
        printk(KERN_ALERT "\n\n\n---------- dma_MAP_ERROR: %pa\n\n\n\n",&bus_addr_sensor_from_device);
    }
    /* --- END DMA MAP SINGE FOR SENSOR DATA FROM DEVICE --- */
    
    /* --- DMA MAP SINGE FOR ACTUATOR DATA TO DEVICE --- */
    if(bus_addr_actuator_to_device != 0)
        dma_unmap_single(dev_ret, bus_addr_actuator_to_device, sizeof(PtolemyActuator_t), DMA_TO_DEVICE);
    
    bus_addr_actuator_to_device = dma_map_single(dev_ret, (PtolemyActuator_t *) PtolemyActuator, sizeof(PtolemyActuator_t), DMA_TO_DEVICE);
    
    if (dma_mapping_error(dev_ret, bus_addr_actuator_to_device)){
        printk(KERN_ALERT "\n\n\n---------- dma_MAP_ERROR: %pa\n\n\n\n",&bus_addr_actuator_to_device);
    }
    /* --- END DMA MAP SINGE FOR ACTUATOR DATA TO DEVICE --- */
      
    return DEVICE_MEMORY_ENTRY_SIZE;
}


void ramdevice_cleanup(void)
{
      kfree(PtolemySensorAttr);
      kfree(PtolemySensorData);
      kfree(PtolemyActuator);
}


//# Interrupt Implementation #
irq_handler_t mydev_isr(int irq, void *device_id, struct pt_regs *regs)
{
  //printk( KERN_ALERT "\n\nINTERRUPT TRIGGERED!!\n\n");
  dma_from_device_completed = 1;

  return (irq_handler_t) IRQ_HANDLED;
}


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    u32 Gem5currentTick; // This variable declares the gem5 simulated time in ms
    
    switch (cmd)
    {
	       
        case QUERY_BUS_ADDR_SENSOR:
	    iowrite32(cpu_to_le32(bus_addr_sensor_from_device), ioremap_res + DMA_TRANSFER_SENSOR_FROM_DEVICE);
            break;    
        
        case QUERY_SENSOR_WRITE_DATA:
            if (copy_from_user(PtolemySensorAttr, (PtolemySensorAttr_t *)arg, sizeof(PtolemySensorAttr_t))){
                return -EACCES;
            }    
            
            dma_from_device_completed = 0;
            
            iowrite32(cpu_to_le32(bus_addr_sensor_to_device), ioremap_res + DMA_TRANSFER_SENSOR_TO_DEVICE);
            break;    
            
            
        case QUERY_SENSOR_READ_DATA:                        
            if (copy_to_user((PtolemySensorData_t *)arg, PtolemySensorData, sizeof(PtolemySensorData_t))){
                return -EACCES;
	    }
            break;  
            
        case DMA_FROM_DEVICE_WAIT:            
            if(dma_from_device_completed == 0)
                ret = 1;
            else
                ret = 0;
            if (copy_to_user((u8 *)arg, (u8 *)&ret, sizeof(u8)))
                return -EACCES;
            break;    
            
        case QUERY_ACTUATOR_WRITE_DATA:
            if (copy_from_user(PtolemyActuator, (PtolemyActuator_t *)arg, sizeof(PtolemyActuator_t))){
                return -EACCES;
            }   
            
            iowrite32(cpu_to_le32(bus_addr_actuator_to_device), ioremap_res + DMA_TRANSFER_ACTUATOR_TO_DEVICE);
            
            break;   
            
        case QUERY_GEM5_CURR_TICK:     
            
            Gem5currentTick = ioread32(ioremap_res + GEM_CURRENT_TICK);
            
            if (copy_to_user((u32 *)arg, (u32 *)&Gem5currentTick, sizeof(u32))){
                return -EACCES;
	    }
            break;      
	    
        default:
            return -EINVAL;
    }
 
    return 0;
}
 
 
 
 
 
 
static struct file_operations query_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
    .ioctl = my_ioctl
#else
    .unlocked_ioctl = my_ioctl
#endif
};



 
static int __init ApollonKernel_driver_init(void)
{
  
    int ret;
     
    if ( ! request_mem_region(BASE, ADDR_SIZE, "Apollon") ) {
	      printk( KERN_ALERT "Unable to get io port at 0x%8X\n", BASE );
	      return -ENODEV;
    }
    
    
    if ( request_irq( IRQ, (irq_handler_t) mydev_isr, IRQF_DISABLED, "Apollon", NULL ) ) {
      printk( KERN_INFO "unable to register IRQ %d\n", IRQ );
      release_mem_region(BASE, ADDR_SIZE);
      return -ENODEV;
    }
    
    printk( KERN_ALERT "\n\nAPOLLON DEVICE: REQUEST KERNEL REGION OK\n\n");
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "ApollonKernel_driver")) < 0)
    {
        return ret;
    }
 
    cdev_init(&c_dev, &query_fops);
 
    if ((ret = cdev_add(&c_dev, dev, MINOR_CNT)) < 0)
    {
        return ret;
    }
     
    if (IS_ERR(cl = class_create(THIS_MODULE, "char")))
    {
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(cl);
    }
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "Apollon")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }
    
    ramdevice_init();
    
    return 0;
}
 
static void __exit ApollonKernel_driver_exit(void)
{
    ramdevice_cleanup();
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);    
    release_mem_region(BASE, ADDR_SIZE);
}
 
module_init(ApollonKernel_driver_init);
module_exit(ApollonKernel_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tampouratzis Nikolaos <ntampouratzis_at_ece_dot_auth_dot_gr>");
MODULE_DESCRIPTION("ApollonKernel ioctl() Char Driver");
