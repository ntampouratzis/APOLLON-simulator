#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>
 
#include "../../../cgem5/src/dev/arm/ApollonStructs.h"


#define QUERY_SENSOR_WRITE_DATA   _IOW('p', 1, PtolemySensorAttr_t *)
#define QUERY_SENSOR_READ_DATA    _IOR('p', 2, PtolemySensorData_t *)
#define QUERY_ACTUATOR_WRITE_DATA _IOW('p', 3, PtolemyActuator_t *)
#define DMA_FROM_DEVICE_WAIT      _IOR('q', 4, uint8_t *)
#define QUERY_GEM5_CURR_TICK      _IOR('p', 5, uint32_t *)
#define QUERY_BUS_ADDR_SENSOR     _IO('q', 6)

#endif