#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <wiringPi.h>
#include "DEV_Config.h"
#include "PCA9685.h"
#include "lineSensor.h"

// function prototypes
void forward(int speed, int motor);
void backward(int speed, int motor);
void turn(int speed);
void stop();
