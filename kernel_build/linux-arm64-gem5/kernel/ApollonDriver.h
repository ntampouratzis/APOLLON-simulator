#ifndef SENSOR_DRIVER_H
#define SENSOR_DRIVER_H

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

#include <stdlib.h>
#include <stdint.h>

void SensorInitialization();
void SensorFinalization();


/* Read the Ptolemy sensor Value                                                                       *
 * (i)   Input: The Instance Name of Sensor (the same name with the Ptolemy environment)               *
 * (ii)  Input: The Attribute Name (the same name with the Ptolemy environment)                        *
 * Output: The value of sensor (in double)                                                             */
double PtolemySensor(char * instanceName, char * attributeName);


/* Write Ptolemy actuator Values                                                                   *
 * (i)   Input: The Instance Name of Sensor (the same name with the Ptolemy environment)               *
 * (ii)  Input: A list of Attribute Names (the same name with the Ptolemy environment)                 *
 * (iii) Input: Values of each attribute Name                                                          *
 * (iv)  Input: Number of attributes                                                                   */
void PtolemyActuator(char * instanceName, char * attributeName[], double attributeValue[], int NoOfAttributes);


/* Returns the gem5 simulated time in ms                                                               */
uint32_t Gem5SimulatedTime();

#endif

