#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <wiringPi.h>
// #include "main.h"

#define LEFT 0
#define MIDDLE 2
#define RIGHT 3

extern int leftLineSensor;
extern int middleLineSensor;
extern int rightLineSensor;

void* line(void* args);
