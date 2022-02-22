#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H
#include <linux/ioctl.h>
 


typedef struct
{
    uint8_t  SensorNum;
    int32_t      value;
    uint8_t  Precision;
} SensorParams_t;


#define QUERY_READ_VALUE 	_IOWR('p', 1, SensorParams_t *)
#define QUERY_DELAY_REQUEST 	_IOW('p', 2, uint64_t *)

#endif