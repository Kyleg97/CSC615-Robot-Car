/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name: Kyle Gilbert
* Student ID: 920913497
* Project: Assignment 5- Follow in Line but Stop
*
* File: lineSensor.c
*
* Description: This program uses both a line sensor and IR object
* sensor. If the line sensor detects a line, the console will print
* that. If the IR object sensor detects an obstructing object, that
* will also be printed in the console. This program uses multiple
* threads (one for each sensor) and updates the data in the main program.
*
**************************************************************/
#include "lineSensor.h"

// initialize the mutex
pthread_mutex_t lineMutex = PTHREAD_MUTEX_INITIALIZER;

int leftLineSensor = 1;
int middleLineSensor = 1;
int rightLineSensor = 1;

typedef struct {
  int lineSensorPin;
} lineSensorArgs;

void* line(void* args) {
    lineSensorArgs *sensorArgs = args;
    int lineSensor = sensorArgs->lineSensorPin;
    pinMode(lineSensor, INPUT);
    // infinite loop
    while(1) {
        // if sensor receives input
        if (digitalRead(lineSensor) == HIGH) {
            // update our data and keep it safe with mutex locking
            if (lineSensor == MIDDLE) {
                pthread_mutex_lock(&lineMutex);
                middleLineSensor = 0;
                pthread_mutex_unlock(&lineMutex);
            } else if (lineSensor == LEFT) {
                pthread_mutex_lock(&lineMutex);
                leftLineSensor = 0;
                pthread_mutex_unlock(&lineMutex);
            } else if (lineSensor == RIGHT) {
                pthread_mutex_lock(&lineMutex);
                rightLineSensor = 0;
                pthread_mutex_unlock(&lineMutex);
            }
        }

        if (digitalRead(lineSensor) == LOW) {
            // update our data and keep it safe with mutex locking
            if (lineSensor == MIDDLE) {
                pthread_mutex_lock(&lineMutex);
                middleLineSensor = 1;
                pthread_mutex_unlock(&lineMutex);
            } else if (lineSensor == LEFT) {
                pthread_mutex_lock(&lineMutex);
                leftLineSensor = 1;
                pthread_mutex_unlock(&lineMutex);
            } else if (lineSensor == RIGHT) {
                pthread_mutex_lock(&lineMutex);
                rightLineSensor = 1;
                pthread_mutex_unlock(&lineMutex);
            }
        }
    }
}

void RouteDecider(){
    if( leftLineSensor == 0 && middleLineSensor == 1 && rightLineSensor == 0 ){
        //run engine straight
        forward(100,1);
    }
    else if( leftLineSensor == 1 && middleLineSensor == 0 && rightLineSensor == 0 ){
        //turn left
        turn(0);
    }
    else if ( leftLineSensor == 0 && middleLineSensor == 0 && rightLineSensor == 1 ){
        //turn right
        turn(1);
    }
    //for following two cases adjust until only the middle sensor runs
    else if( leftLineSensor == 1 && middleLineSensor == 1 && rightLineSensor == 0 ){
        //adjust left
    }
    else if ( leftLineSensor == 0 && middleLineSensor == 1 && rightLineSensor == 1 ){
        //adjust right 
    }
    else if( leftLineSensor == 0 && middleLineSensor == 0 && rightLineSensor == 0 ){
        //find the line
    }

    //following cases unlikely to happen but should be considered

    else if( leftLineSensor == 1 && middleLineSensor == 1 && rightLineSensor == 1 ){
        //somehow all the sensors are on the line
        //possible case at a corner and car is slightly off center
    }
    else if( leftLineSensor == 1 && middleLineSensor == 0 && rightLineSensor == 1 ){
        //lol how would this happen
    }

}

