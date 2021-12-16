#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>


//Left Echo
#define LEFTECHOT 8
#define LEFTECHOE 9
//Center Echo
#define CENTERECHOT 15
#define CENTERECHOE 16
//Right Echo
#define RIGHTECHOT 12
#define RIGHTECHOE 13


extern int objectLeft;
extern int objectFront;
extern int objectRight;


long getMicroSeconds();

void* objectDetector(void* args);
