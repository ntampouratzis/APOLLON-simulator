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
 
#include "SensorKernel_driver.h"

#include <linux/ioport.h>
#include <linux/interrupt.h> //# Interrupt Implementation #
#include <asm/io.h>


#define IRQ 54 //# Interrupt Implementation #
 
#define FIRST_MINOR 0
#define MINOR_CNT 1



#define BASE 0x1c061000
#define ADDR_SIZE 0xfff

#define SENSOR_NUMBER_BASE        0x00000
#define SENSOR_NUMBER_OFFSET      0x00008

 
static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
struct device *dev_ret;



 
static int my_open(struct inode *i, struct file *f)
{
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    return 0;
}



//# Interrupt Implementation #
irq_handler_t mydev_isr(int irq, void *device_id, struct pt_regs *regs)
{

  return (irq_handler_t) IRQ_HANDLED;
}


#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,35))
static int my_ioctl(struct inode *i, struct file *f, unsigned int cmd, unsigned long arg)
#else
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
#endif
{
    SensorParams_t params;
    u64 delay_val;
 
    switch (cmd)
    {
	    
        case QUERY_DELAY_REQUEST:
	    if (copy_from_user((u64 *)&delay_val, (u64 *)arg, sizeof(u64))){
                return -EACCES;
            }
            
	    msleep(delay_val);
	    
            break;
	    
	    
	case QUERY_READ_VALUE:
            if (copy_from_user(&params, (SensorParams_t *)arg, sizeof(SensorParams_t))){
                return -EACCES;
            }            
            
	    
	    if(SENSOR_NUMBER_BASE + ((params.SensorNum-1)*SENSOR_NUMBER_OFFSET) > ADDR_SIZE){
	      printk( KERN_ALERT "The (SENSOR_NUMBER_BASE + (params.SensorNum*SENSOR_NUMBER_OFFSET)):0x%8X is out of range! ADDR_SIZE is 0x%8X\n", SENSOR_NUMBER_BASE + ((params.SensorNum-1)*SENSOR_NUMBER_OFFSET), ADDR_SIZE);
	      return -ENODEV;
	    }
	    
	    iowrite8((uint8_t) params.Precision, ioremap(BASE, ADDR_SIZE) + SENSOR_NUMBER_BASE + ((params.SensorNum-1)*SENSOR_NUMBER_OFFSET));
	    params.value = (int32_t) ioread32(ioremap(BASE, ADDR_SIZE) + SENSOR_NUMBER_BASE + ((params.SensorNum-1)*SENSOR_NUMBER_OFFSET));
	    
	    if (copy_to_user((SensorParams_t *)arg, &params, sizeof(SensorParams_t))){
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



 
static int __init SensorKernel_driver_init(void)
{
  
    int ret;
     
    if ( ! request_mem_region(BASE, ADDR_SIZE, "Sensor") ) {
	      printk( KERN_ALERT "Unable to get io port at 0x%8X\n", BASE );
	      return -ENODEV;
    }
    
    
    if ( request_irq( IRQ, (irq_handler_t) mydev_isr, IRQF_DISABLED, "Sensor", NULL ) ) {
      printk( KERN_INFO "unable to register IRQ %d\n", IRQ );
      release_mem_region(BASE, ADDR_SIZE);
      return -ENODEV;
    }
    
    
    
 
    if ((ret = alloc_chrdev_region(&dev, FIRST_MINOR, MINOR_CNT, "SensorKernel_driver")) < 0)
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
    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "Sensor")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, MINOR_CNT);
        return PTR_ERR(dev_ret);
    }
    
    
    
    return 0;
}
 
static void __exit SensorKernel_driver_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, MINOR_CNT);
    
    release_mem_region(BASE, ADDR_SIZE);
}
 
module_init(SensorKernel_driver_init);
module_exit(SensorKernel_driver_exit);
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tampouratzis Nikolaos <ntampouratzis_at_isc_dot_tuc_dot_gr>");
MODULE_DESCRIPTION("SensorKernel ioctl() Char Driver");
