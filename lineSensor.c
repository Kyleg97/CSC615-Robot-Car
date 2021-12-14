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
