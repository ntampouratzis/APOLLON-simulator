#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include <stdlib.h>
#include <stdint.h>

typedef int InitDescr;

InitDescr SensorInitialization();
void SensorFinalization(InitDescr fd);

/* Read the sensor Value                                                                               *
 * (i)   Input: Initialization Descriptor (this parameter is given from SensorInitialization function) *
 * (ii)  Input: The number of Sensor                                                                   *
 * (iii) Input: The precision value. In other words, the numbers after the comma of float number       *
 * Output: The value of sensor                                                                         */
float SensorRead(InitDescr fd, uint8_t SensorNum, uint8_t Precision);

/* Pauses the program for the amount of time (in miliseconds) specified as parameter.                 *
 * (There are 1000 milliseconds in a second.)                                                         *
 * (i)  Input: Initialization Descriptor (this parameter is given from SensorInitialization function) *
 * (ii) Input: the number of milliseconds to pause                                                    */
void delay(InitDescr fd, uint64_t val);


