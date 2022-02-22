#include "ApollonDriver.h"

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

#include <inttypes.h>

int main(int argc, char *argv[])
{
    
    SensorInitialization();
    
    uint64_t loop_count = 100; // 100ms example
    uint64_t i;
    
    struct timeval start, end;
    
    for(i=0;i<loop_count;i++){
        
        char * attributeName[1];
        double attributeValue[1];
        attributeName[0]  = "GetTemperature";
        attributeValue[0] = 1;
        PtolemyActuator("actuator1", attributeName, attributeValue, 1);
        
        usleep(200); // waits for 200us
        
        printf("Iteration:%" PRIu64 "\n", i);
        
        /*gettimeofday(&start, NULL);
        printf("\n --- Gem5SimulatedTime: %d ms ---\n", Gem5SimulatedTime());*/
        
        double TemperatureOfSensor1 = PtolemySensor("sensor1", "Temperature");
        printf("The value of Temperature Of Sensor1 is: %f\n",TemperatureOfSensor1);

        /*gettimeofday(&end, NULL);

        printf("%ld\n", ((end.tv_sec * 1000000 + end.tv_usec)
		  - (start.tv_sec * 1000000 + start.tv_usec)));
        */
        
        usleep(800); // waits for 800us
    
    }
    
    
    SensorFinalization();
 
    return 0;
}
