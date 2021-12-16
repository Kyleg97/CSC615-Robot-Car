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
#include "echo.h"

// used if the program is quit
// while the motor is running
void Handler(int signo) {
  printf("Motor Stop\n");
  PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, 0);
  PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, 0);
  exit(0);
}

// 1 is left, 0 is right
int lastTurned = 1;

typedef struct {
  int lineSensorPin;
} lineSensorArgs;

typedef struct{
	int echoSensorTrig;
	int echoSensorEcho;
} echoSensorArgs;


void RouteDecider(int speed) {
    // if the car is positioned straight on the line (middle sensor on line, outer off line)
    if (leftLineSensor == 0 && middleLineSensor == 1 && rightLineSensor == 0) {
        forward(speed, 1);
    // if the left sensor is on the line, and the middle/right sensors off line
    } else if(leftLineSensor == 1 && middleLineSensor == 0 && rightLineSensor == 0) {
        //turn left
        printf("turning left...1\n");
        stop();
        turn(1, speed);
        lastTurned = 1;
        while(leftLineSensor == 1);
        forward(speed, 1);
    // if the right sensor is on the line, and the other 2 are off the line
    } else if (leftLineSensor == 0 && middleLineSensor == 0 && rightLineSensor == 1) {
        //turn right
        printf("turning right...1\n");
        stop();
        turn(0, speed);
        lastTurned = 0;
        while(rightLineSensor == 1);
        forward(speed, 1);
    // if both the left/middle are on line, adjust to center car
    } else if (leftLineSensor == 1 && middleLineSensor == 1 && rightLineSensor == 0) {
        //adjust left
        printf("turning left...2\n");
        stop();
        turn(1, speed);
        lastTurned = 1;
        while(leftLineSensor == 1);
        forward(speed, 1);
    // if both the right/middle are on line, adjust to center car
    } else if (leftLineSensor == 0 && middleLineSensor == 1 && rightLineSensor == 1) {
        //adjust right
        printf("turning right...3\n");
        stop();
        turn(0, speed);
        lastTurned = 0;
        while(rightLineSensor == 1);
        forward(speed, 1);
    // if all sensors are off the line, turn and hope we find it
    } else if (leftLineSensor == 0 && middleLineSensor == 0 && rightLineSensor == 0) {
        // find the line
        // turn in place
        printf("turning in place...\n");
        stop();
        if (lastTurned == 0) {
          turn(0, speed);
        } else if (lastTurned == 1) {
          turn(1, speed);
        }
        while(middleLineSensor != 1);
        forward(speed, 1);
    // if all three sensors are on the line (probably stuck turning)
    } else if(leftLineSensor == 1 && middleLineSensor == 1 && rightLineSensor == 1) {
        /*stop();
        turn(0, 50);
        while(middleLineSensor != 1 && leftLineSensor == 1 && rightLineSensor == 1);*/
    // if outer sensors are on line but middle isnt (probably stuck on a turn)
    } else if(leftLineSensor == 1 && middleLineSensor == 0 && rightLineSensor == 1) {
        printf("no sensors see a line!\n");
        stop();
        if (lastTurned == 0) {
          turn(1, speed);
        } else if (lastTurned == 1) {
          turn(0, speed);
        }
        while(middleLineSensor != 1);
        forward(speed, 1);

    }
}

int main() {
    // setup
    wiringPiSetup();
    printf("wiringpi setted up\n");
    PCA9685_Init(0x40);
    PCA9685_SetPWMFreq(100);

    printf("pca init done\n");
    pthread_t lineThreads[3];
    // create thread for line sensors
    printf("linethread creating\n");
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

    printf("echo thread creating\n");
    pthread_t detector[3];
    for(int i = 0; i < 3; i++){
		echoSensorArgs *args = malloc(sizeof *args);
		if( i == 0 ){//center echo
			printf("Creating thread 1, %d,%d\n", CENTERECHOT, CENTERECHOE);
			args->echoSensorTrig = CENTERECHOT;
			args->echoSensorEcho = CENTERECHOE;
		}
		if( i == 1){//left
			printf("Creating thread 2\n");
			args->echoSensorTrig = 8;
			args->echoSensorEcho = 9;
		}
		if( i == 2){//right
			printf("Creating thread 3\n");
			args->echoSensorTrig = 12;
			args->echoSensorEcho = 13;
		}
		pthread_create(&detector[i], NULL, objectDetector, args);
    }

//    for(int i = 0; i < 3; i++){
//		pthread_join(detector[i], NULL);
//    }

    signal(SIGINT, Handler);

    while(1) {
      RouteDecider(50);
    }

    DEV_ModuleExit();

    return 0;
}
