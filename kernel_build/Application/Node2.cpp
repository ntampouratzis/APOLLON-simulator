#include "ApollonDriver.h"

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#include <fstream>
#include <sstream>

/* Node 2 */

typedef struct SensorValues{
 double value;
 bool final_val;
}SensorValue;

int Node1TCPInitialization(int argc, char *argv[]){
    int sockfd = 0;

    struct sockaddr_in serv_addr; 

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 

    //memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(6000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 
    
    return sockfd;
}


SensorValue Node1RcvValues(int sockfd){
    SensorValue sensor;
    int n = 0;
    n = read(sockfd, (void *) &sensor, sizeof(sensor));
    if(n < 0)
        printf("\n Read error \n");
    return sensor;
}




int main(int argc, char *argv[])
{
    SensorInitialization();
    int sockfd = Node1TCPInitialization(argc, argv);
    
    //std::ofstream timeseries;
    //timeseries.open("timeseriesNode1.txt", std::ios::out | std::ios::app); 

    int i = 1;
    while(1){
        SensorValue sensor = Node1RcvValues(sockfd);
        if(sensor.value != 0.0){
            printf("value (%d): %f | final_val: %d | \n",i, sensor.value, sensor.final_val);
            
            char * attributeName[1];
            double attributeValue[1];
            attributeName[0]  = "GetPrediction";
            attributeValue[0] = sensor.value;
            PtolemyActuator("actuator1", attributeName, attributeValue, 1);
        }
        
        //timeseries << std::scientific << sensor.value << std::endl;
        
        if(sensor.final_val == true)
            break;
      
        i++;
        usleep(500);
    }
    
    //timeseries.close();
    SensorFinalization();

    return 0;
}