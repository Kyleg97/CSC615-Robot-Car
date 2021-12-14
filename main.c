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

    // forward(70, 1);

    // infinite loop for when the motor
    // reaches the backward function to
    // run until program is quit
    while(1) {

    }

    DEV_ModuleExit();

    return 0;
}
