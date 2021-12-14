/**************************************************************
* Class: CSC-615-01 Fall 2021
* Name: Kyle Gilbert
* Student ID: 920913497
* Project: Car Project
*
* File: main.c
*
*
**************************************************************/

#include "main.h"
#include "avoidanceSensor.h"
#include "lineSensor.h"
#include "motor.h"

// used if the program is quit
// while the motor is running
void Handler(int signo) {
  printf("Motor Stop\n");
  PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, 0);
  PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, 0);
  exit(0);
}

typedef struct {
  int lineSensorPin;
} lineSensorArgs;

void RouteDecider() {
    if (leftLineSensor == 0 && middleLineSensor == 1 && rightLineSensor == 0) {
        forward(75, 1);
    } else if(leftLineSensor == 1 && middleLineSensor == 0 && rightLineSensor == 0) {
        //turn left
        turn(0, 75);
        while(leftLineSensor == 1);
        forward(75, 1);
    } else if (leftLineSensor == 0 && middleLineSensor == 0 && rightLineSensor == 1) {
        //turn right
        turn(1, 75);
        while(rightLineSensor == 1);
        forward(75, 1);
    } else if (leftLineSensor == 1 && middleLineSensor == 1 && rightLineSensor == 0) {
        //adjust left
        turn(1, 75);
        while(leftLineSensor == 1);
        forward(75, 1);
    } else if (leftLineSensor == 0 && middleLineSensor == 1 && rightLineSensor == 1) {
        //adjust right
        turn(0, 75);
        while(rightLineSensor == 1);
        forward(75, 1);
    } else if (leftLineSensor == 0 && middleLineSensor == 0 && rightLineSensor == 0) {
        //find the line
    } else if(leftLineSensor == 1 && middleLineSensor == 1 && rightLineSensor == 1) {
        //somehow all the sensors are on the line
        //possible case at a corner and car is slightly off center
    } else if(leftLineSensor == 1 && middleLineSensor == 0 && rightLineSensor == 1) {
        //lol how would this happen
    }

}

int main() {
    // setup
    wiringPiSetup();
    PCA9685_Init(0x40);
    PCA9685_SetPWMFreq(100);

    pthread_t lineThreads[3];
    // create thread for line sensors
    for (int i = 0; i < 3; i++) {
      lineSensorArgs *args = malloc(sizeof *args);
      if (i == 0) {
        args->lineSensorPin = 0;
      } else if (i == 1) {
        args->lineSensorPin = 2;
      } else if (i == 2) {
        args->lineSensorPin = 3;
      }
      pthread_create(&lineThreads[i], NULL, line, args);
    }

    signal(SIGINT, Handler);

    /* forward(70, 1);
    sleep(2);
    stop();
    sleep(2);
    turn(1);
    sleep(2);
    stop();
    sleep(2);
    forward(70,1);
    sleep(2);
    stop();
    sleep(2);
    turn(0);
    sleep(2);
    stop(); */

    forward(75, 1);

    // infinite loop for when the motor
    // reaches the backward function to
    // run until program is quit
    while(1) {
      RouteDecider();
    }

    DEV_ModuleExit();

    return 0;
}
