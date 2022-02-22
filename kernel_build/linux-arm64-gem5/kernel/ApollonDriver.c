#include "ApollonDriver.h"

#include "ApollonKernel_driver.h"

#include <sys/stat.h>




void CreateDevice(char * dev);
int doesDeviceExist(const char *filename);
void dma_from_device_wait();


int fd;


int doesDeviceExist(const char *filename) {
    struct stat st;
    int result = stat(filename, &st);
    return result == 0;
}

void CreateDevice(char * dev){
  //! Check if the Device already exists !//
  char file_exist[32] = "/dev/";
  strcat(file_exist,dev);
  if(doesDeviceExist(file_exist)){
    return;
  }
  
  char cmd[32] = "cat sys/class/char/";
  strcat(cmd,dev);
  strcat(cmd,"/dev");
  
  FILE *fp;
  char path[32];
  
  //! Open the command for reading. !//
  fp = popen(cmd, "r");
  if (fp == NULL) {
    printf("Failed to run cat sys/class/char command\n" );
    exit(1);
  }

  //! Read the output to find the MAJOR & MINOR NUMBER OF DEVICE and then allocate the device using mknod !//
  if (fgets(path, sizeof(path)-1, fp) != NULL) {
    char mknod_str[32] = "mknod /dev/";
    strcat(mknod_str,dev);
    strcat(mknod_str," c ");
    strcat(mknod_str,strtok (path,":"));
    strcat(mknod_str," ");
    strcat(mknod_str,strtok (NULL,":"));
    system(mknod_str);
  }
  else{
   printf("The Kernel Driver does not exists!\n"); 
  }

  //! close !//
  pclose(fp);
}

void PtolemyActuator(char * instanceName, char * attributeName[], double attributeValue[], int NoOfAttributes){
    if(NoOfAttributes > MAX_ACTUATOR_ATTRIBUTES){
        perror("\n\n PtolemyActuator ERROR: NoOfAttributes > MAX_ACTUATOR_ATTRIBUTES\n\n");
    }
    
    PtolemyActuator_t PtolemyAct;
    
    PtolemyAct.NoOfActiveAttributes = NoOfAttributes; //Set the number of active attributes
    // Copy the Data to PtolemyAct struct
    strcpy(PtolemyAct.instanceName, instanceName);
    for(int i=0; i<NoOfAttributes;i++){
        strcpy(PtolemyAct.attributeName[i], attributeName[i]);
        PtolemyAct.value_double[i] = attributeValue[i];
    }
    
    if (ioctl(fd, QUERY_ACTUATOR_WRITE_DATA, &PtolemyAct) == -1){
        perror("\n\n IOCTL ERROR: QUERY_ACTUATOR_WRITE_DATA\n\n");
    }
}


double PtolemySensor(char * instanceName, char * attributeName){
    PtolemySensorAttr_t PtolemySensorAttr;
    PtolemySensorData_t PtolemySensorData;
    
    strcpy(PtolemySensorAttr.instanceName, instanceName);
    strcpy(PtolemySensorAttr.attributeName, attributeName);
    
    if (ioctl(fd, QUERY_SENSOR_WRITE_DATA, &PtolemySensorAttr) == -1){
        perror("\n\n IOCTL ERROR: QUERY_SENSOR_WRITE_DATA\n\n");
    }
    
    dma_from_device_wait();
    
    if (ioctl(fd, QUERY_SENSOR_READ_DATA, &PtolemySensorData) == -1){
        perror("\n\n IOCTL ERROR: QUERY_SENSOR_READ_DATA\n\n");
    }
    
    return PtolemySensorData.value_double;
}

uint32_t Gem5SimulatedTime(){
    uint32_t Gem5currentTick; // This variable declares the gem5 simulated time in ms
        
    if (ioctl(fd, QUERY_GEM5_CURR_TICK, (uint32_t *) &Gem5currentTick) == -1){
        perror("\n\n IOCTL ERROR: QUERY_GEM5_STATISTICS\n\n");
    }
    
    return Gem5currentTick;
}


void dma_from_device_wait()
{
    uint8_t ret;
    if (ioctl(fd, DMA_FROM_DEVICE_WAIT, (uint8_t *) &ret) == -1){
        perror("query_apps ioctl dma_from_device_wait");
    }
    
    if(ret == 1){
      dma_from_device_wait();
    }
}


void SensorInitialization(){
  char * dev = "Apollon";
  CreateDevice(dev);
  char file_name[16] = "/dev/";
  strcat(file_name,dev);
  fd = open(file_name, O_RDWR);
  if (fd == -1){
    perror("Apollon Device open");
    return;
  }  
  if (ioctl(fd, QUERY_BUS_ADDR_SENSOR) == -1){
        perror("\n\n IOCTL ERROR: QUERY_BUS_ADDR_SENSOR\n\n");
  }
  return;
}

void SensorFinalization(){
  close (fd);
}
 
