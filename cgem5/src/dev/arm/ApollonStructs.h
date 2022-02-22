#ifndef APOLLON_STRUCTS_H
#define APOLLON_STRUCTS_H

#define MAX_ACTUATOR_ATTRIBUTES 10

/* This struct used to transfer the data from Simulated System to Gem5 Device *
 * Expand it if you would like to add more variables                          *
 */
typedef struct
{
    char instanceName[64];
    char attributeName[64];
} PtolemySensorAttr_t;

/* The following structs used to transfer the data from Gem5 Device to Simulated System *
 * Expand it if you would like to add more variables                          *
 */
typedef struct
{
    double value_double;
} PtolemySensorData_t;


typedef struct
{
    char instanceName[64];
    char attributeName[MAX_ACTUATOR_ATTRIBUTES][64];
    double value_double[MAX_ACTUATOR_ATTRIBUTES];
    int NoOfActiveAttributes;
} PtolemyActuator_t;

#endif