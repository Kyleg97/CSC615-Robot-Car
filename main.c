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

// function prototypes
void forward(int speed, int motor);
void backward(int speed, int motor);
void stop();

// used if the program is quit
// while the motor is running
void Handler(int signo) {
  printf("Motor Stop\n");
  PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, 0);
  PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, 0);
  exit(0);
}
// 0 is left motor, 1 is right motor
void forward(int speed, int motor) {
    printf("Running forwards...\n");
    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, speed);
    PCA9685_SetLevel(PCA_CHANNEL_1, 1);
    PCA9685_SetLevel(PCA_CHANNEL_2, 0);

    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, speed);
    PCA9685_SetLevel(PCA_CHANNEL_3, 0);
    PCA9685_SetLevel(PCA_CHANNEL_4, 1);
    while(1) {
      if (leftLineSensor == 0) {
        printf("we need to turn right");
      }
      if (middleLineSensor == 0) {
        printf("we're in the middle");
      }
      if (rightLineSensor == 0) {
        printf("we need to turn left");
      }
    }
}

// 0 is left motor, 0 is right motor
void backward(int speed, int motor) {
    printf("Running backwards...\n");
    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, speed);
    PCA9685_SetLevel(PCA_CHANNEL_1, 1);
    PCA9685_SetLevel(PCA_CHANNEL_2, 0);
}
void turn(int Dir){
    if( Dir == 0 ){// 0 for left turn and 1 for right 
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, 100);
        PCA9685_SetLevel(PCA_CHANNEL_1, 1);
        PCA9685_SetLevel(PCA_CHANNEL_2, 0);

        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, speed);
        PCA9685_SetLevel(PCA_CHANNEL_3, 1);
        PCA9685_SetLevel(PCA_CHANNEL_4, 0);

    }
    else if( Dir == 1 ){
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, 100);
        PCA9685_SetLevel(PCA_CHANNEL_1, 1);
        PCA9685_SetLevel(PCA_CHANNEL_2, 0);

        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, speed);
        PCA9685_SetLevel(PCA_CHANNEL_3, 1);
        PCA9685_SetLevel(PCA_CHANNEL_4, 0);
    }
    else{
        printf("Neither right or left turn direction specified\n");
    }

}

// stops the motor
void stop() {
    printf("Stopping motor...\n");
    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, 0);
    sleep(2); // wait 2 seconds
}

// functions for setting data
/*void setLineData(int data) {
    lineData = data;
}*/

/*void setObjectData(int data) {
    objectData = data;
}*/

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
    // pthread_create(&lineThread1, NULL, &line, NULL);
    // pthread_create(&lineThread2, NULL, &line, NULL);
    // pthread_create(&lineThread3, NULL, &line, NULL);
    // create thread for IR object sensor
    // pthread_create(&thread2, NULL, &avoidance, NULL);
    // infinite loop until program is terminated
    /* while(1) {
        // if line thread received input and set our variable
        if (lineData == 1) {
            printf("line detected\n");
        }
        // same as above but for the IR object sensor
        if (objectData == 1) {
            printf("object detected\n");
        }

    }*/

    signal(SIGINT, Handler);

    forward(70, 1);

    // infinite loop for when the motor
    // reaches the backward function to
    // run until program is quit
    while(1) {

    }

    DEV_ModuleExit();

    return 0;
}
