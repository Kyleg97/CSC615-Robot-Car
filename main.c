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
    // if the car is positioned straight on the line (middle sensor on line, outer off line)
    if (leftLineSensor == 0 && middleLineSensor == 1 && rightLineSensor == 0) {
        forward(50, 1);
    // if the left sensor is on the line, and the middle/right sensors off line
    } else if(leftLineSensor == 1 && middleLineSensor == 0 && rightLineSensor == 0) {
        //turn left
        stop();
        turn(0, 50);
        while(leftLineSensor == 1);
        forward(50, 1);
    // if the right sensor is on the line, and the other 2 are off the line
    } else if (leftLineSensor == 0 && middleLineSensor == 0 && rightLineSensor == 1) {
        //turn right
        stop();
        turn(1, 50);
        while(rightLineSensor == 1);
        forward(50, 1);
    // if both the left/middle are on line, adjust to center car
    } else if (leftLineSensor == 1 && middleLineSensor == 1 && rightLineSensor == 0) {
        //adjust left
        stop();
        turn(1, 50);
        while(leftLineSensor == 1);
        forward(50, 1);
    // if both the right/middle are on line, adjust to center car
    } else if (leftLineSensor == 0 && middleLineSensor == 1 && rightLineSensor == 1) {
        //adjust right
        stop();
        turn(0, 50);
        while(rightLineSensor == 1);
        forward(50, 1);
    // if all sensors are off the line, turn and hope we find it
    } else if (leftLineSensor == 0 && middleLineSensor == 0 && rightLineSensor == 0) {
        // find the line
        // turn in place
        stop();
        turn(0, 50);
        while(middleLineSensor != 1 && leftLineSensor == 1 && rightLineSensor == 1);
    // if all three sensors are on the line (probably stuck turning)
    } else if(leftLineSensor == 1 && middleLineSensor == 1 && rightLineSensor == 1) {
        stop();
        turn(0, 50);
        while(middleLineSensor != 1 && leftLineSensor == 1 && rightLineSensor == 1);
    // if outer sensors are on line but middle isnt (probably stuck on a turn)
    } else if(leftLineSensor == 1 && middleLineSensor == 0 && rightLineSensor == 1) {
        stop();
        turn(0, 50);
        while(middleLineSensor != 1 && leftLineSensor == 1 && rightLineSensor == 1);
    }
}

void square() {
  for (int i = 0; i < 4; i++) {
    forward(75, 1);
    sleep(1);
    stop();
    sleep(1);
    turn(1, 75);
    sleep(1);
    stop();
    sleep(1);
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
    // square();
    // forward(75, 1);

    // infinite loop for when the motor
    // reaches the backward function to
    // run until program is quit
    printf("before route decider while loop...");
    while(1) {
      RouteDecider();
    }

    DEV_ModuleExit();

    return 0;
}
