/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name: Kyle Gilbert
* Student ID: 920913497
* Project: Robot Car
*
* File: avoidanceSensor.c
*
**************************************************************/
#include "avoidanceSensor.h"

// initialize mutex
pthread_mutex_t objectMutex = PTHREAD_MUTEX_INITIALIZER;

void* avoidance(void* args) {
    pinMode(AVOIDANCESENSOR, INPUT);
    // infinite loop
    while(1) {
        // if sensor receives input, update data
        // and keep it safe/consistent with mutex
        if (digitalRead(AVOIDANCESENSOR) == HIGH) {
            pthread_mutex_lock(&objectMutex);
            // setObjectData(0);
            pthread_mutex_unlock(&objectMutex);
        }
        if (digitalRead(AVOIDANCESENSOR) == LOW) {
            pthread_mutex_lock(&objectMutex);
            // setObjectData(1);
            pthread_mutex_unlock(&objectMutex);
        }
    }
}
