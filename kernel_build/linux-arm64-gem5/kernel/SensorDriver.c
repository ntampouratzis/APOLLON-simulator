#include "SensorDriver.h"

#include "SensorKernel_driver.h"

#include <sys/stat.h>




void CreateDevice(char * dev);
int doesDeviceExist(const char *filename);


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


 

float pow_mine (float base, uint8_t exponent){
  int j = 0;
  float res = 1;
  for(j=0;j<exponent;j++)
    res*=base;
  return res;
}

float SensorRead(InitDescr fd, uint8_t SensorNum, uint8_t Precision){
  
  SensorParams_t params;
  params.SensorNum = SensorNum;
  params.Precision = Precision;
  
  if (ioctl(fd, QUERY_READ_VALUE, &params) == -1){
        perror("query_apps ioctl Sensorparams");
  }
  
  return (float)(params.value/pow_mine(10.0,Precision));
}




void delay(InitDescr fd, uint64_t val){
  if (ioctl(fd, QUERY_DELAY_REQUEST, (uint64_t *) &val) == -1){
        perror("query_apps ioctl delay_request");
  }
}


InitDescr SensorInitialization(){
  char * dev = "Sensor";
  CreateDevice(dev);
  char file_name[16] = "/dev/";
  strcat(file_name,dev);
  InitDescr fd = open(file_name, O_RDWR);
  if (fd == -1){
    perror("Sensor Device open");
    return fd;
  }  
}

void SensorFinalization(InitDescr fd){
  close (fd);
}
 
