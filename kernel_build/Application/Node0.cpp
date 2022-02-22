#include "ApollonDriver.h"

#include <time.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

//#define timeseriesNumber 2
#define measurementPool  20076 // 6*14*239
#define measurementVals  14


typedef struct SensorValues{
 double value;
 bool final_val;
}SensorValue;


int Node1TCPInitialization(){
    int listenfd = 0, connfd = 0;
    struct sockaddr_in serv_addr; 

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(5000); 

    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

    listen(listenfd, 10);
    
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 
    
    printf("\n\n Client 1 connected!! \n\n");
    
    return connfd;
    
}


void Node1SendValues(int connfd, double value, bool final_val){
    SensorValue sensor;
    sensor.value     = value;
    sensor.final_val = final_val;
    
    write(connfd, (void *) &sensor, sizeof(sensor)); 
    
}

int main(int argc, char *argv[]){
    
    SensorInitialization();
    int connfd = Node1TCPInitialization();

    unsigned timeseriesNumber = atoi(argv[1]);
    //std::cout << timeseriesNumber << std::endl;

    const unsigned allMeasurementVals =
        measurementPool + (timeseriesNumber*measurementVals);

    double meanVals[14] =
        {988.88635885, 9.07734895, 283.14631345, 4.44854725, 75.35405895, 13.3829553, 9.29695535,
        4.08591725, 5.8752111, 9.40521005, 1217.51429655, 2.1510037, 3.56950915, 176.21703355};

    double stdVals[14] =
        {8.48043388, 8.85249908, 8.95324185, 7.16584991, 16.72731652, 7.68914559, 4.19808168,
        4.84034436, 2.66564926, 4.25206364, 42.48884277, 1.53666449, 2.33067298, 86.61322998};
    
    double val[measurementVals];

    std::vector<double> measPoolVector;

    std::ofstream timeseries;
    timeseries.open("timeseries.txt", std::ios::out | std::ios::app); 
    
    
    for (int i=0; i<allMeasurementVals; i+=measurementVals){

        // Acquire one measurement which has 14 values
        // We simulate this by reading 14 values (lines) from aPtolemy
        for (int k=0; k<measurementVals; k++){
            char * attributeName[1];
            double attributeValue[1];
            attributeName[0]  = "GetTemperature";
            attributeValue[0] = 1;
            PtolemyActuator("actuator1", attributeName, attributeValue, 1);
            usleep(200); // waits for 200us
            
            val[k] = PtolemySensor("sensor1", "Temperature");
            usleep(300); // waits for 800us
        }

        // Normalize values
        for (int k=0; k<measurementVals; k++){
            val[k] -= meanVals[k];
            val[k] /= stdVals[k];
        }

        // Save measurement values to measurement pool
        for (int k=0; k<measurementVals; k++){
            measPoolVector.push_back(val[k]);
        }

        // if measurement pool size can provide
        // one time-series then create this time-series
        if ( measPoolVector.size() == (measurementPool+measurementVals) ){
            // open timeseries file
            if (timeseries.is_open()){
                // append the timeseries to file
                // hourly values are written
                for ( int l=0; l<(measurementPool+measurementVals); l+=(6*measurementVals) ){
                    for ( int k=0; k<measurementVals; k++ ){
                        timeseries << std::scientific << measPoolVector[l+k] << std::endl;
                        Node1SendValues(connfd, measPoolVector[l+k], false);
                        usleep(500);
                    }
                }
            }
            else{
                std::cout << "Error - could not open file for writing timeseries result." << std::endl;
                return -1;
            }
            // remove the top measurement because it is not going
            // to be used again
            measPoolVector.erase (measPoolVector.begin(), measPoolVector.begin()+measurementVals);
        }

    }

    SensorFinalization();
    
    Node1SendValues(connfd, 0, true);
    close(connfd);

    // close the timeseries file
    timeseries.close();

    return 0;
}