/*
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
*/

#include "echo.h"

//Left Echo
#define LEFTECHOT 8
#define LEFTECHOE 9
//Center Echo
#define CENTERECHOT 15
#define CENTERECHOE 16
//Right Echo
#define RIGHTECHOT 12
#define RIGHTECHOE 13

int objectLeft = 0;
int objectFront = 0;
int objectRight = 0;

pthread_mutex_t echoMutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct{
	int echoSensorTrig;
	int echoSensorEcho;
} echoSensorArgs;

//function uses gettimeofday() to record time begin and end
long getMicroSeconds(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

void* objectDetector(void* args){
	double sensorStatus;
	double distance = 0;
	double soundSpeed = 0.034; //centimeters per microsecond
	long begin, end;
	int counter = 0;
	printf("Just finish initialzing variables\n");

	//initing struct function
	echoSensorArgs *echoArgs = args;
	int sensorTrig = echoArgs->echoSensorTrig; //stores gpio pinnumber
	int sensorEcho = echoArgs->echoSensorEcho; //stores gpio pinnumber


	pinMode(sensorTrig, OUTPUT);
	digitalWrite(sensorTrig, LOW);
	pinMode(sensorEcho, INPUT);

	printf("going into whileloop\n");

	while(counter < 5){
		delay(300);//delay from over printing
		digitalWrite(sensorTrig, HIGH);
		delay(0.1);
		digitalWrite(sensorTrig, LOW);

		int firstEcho, secondEcho, low, high;
		firstEcho = secondEcho = low = high = 0;

		while(low == 0 || high == 0){ //waits to store both values begin and end to calculate distance
			secondEcho = firstEcho;
			firstEcho = digitalRead(sensorEcho);
			if(low == 0 && secondEcho == 0 && firstEcho == 1){
				low = 1;
				begin = getMicroSeconds();
			}
			if(low == 1 && secondEcho == 1 && firstEcho == 0){
				high = 1;
				end = getMicroSeconds();
			}
		}

		//Distance Calculation
		long timePassed = end - begin;
		distance = (timePassed / 2)* soundSpeed;
		sensorStatus = distance;
		
		//Update echo status
		if(sensorStatus > 25.00){
			if(sensorTrig == LEFTECHOT){
				printf("%f, NOTHING TO THE LEFT\n", sensorStatus);
				pthread_mutex_lock(&echoMutex);
				objectLeft = 0;
				pthread_mutex_unlock(&echoMutex);
			}
			else if(sensorTrig == CENTERECHOT){
				printf("%f, NOTHING IN FRONT\n", sensorStatus);
				pthread_mutex_lock(&echoMutex);
				objectFront = 0;
				pthread_mutex_unlock(&echoMutex);
			}
			else if(sensorTrig == RIGHTECHOT){
				printf("%f, NOTHING TO THE RIGHT\n", sensorStatus);
				pthread_mutex_lock(&echoMutex);
				objectRight = 0;
				pthread_mutex_unlock(&echoMutex);
			}

		}

		if(sensorStatus < 25.00){
			if(sensorTrig == LEFTECHOT){
				printf("%f, SOMETHING TO THE LEFT\n", sensorStatus);
				pthread_mutex_lock(&echoMutex);
				objectLeft = 1;
				pthread_mutex_unlock(&echoMutex);
			}
			else if(sensorTrig == CENTERECHOT){
				printf("%f, SOMETHING IN FRONT!\n", sensorStatus);
				pthread_mutex_lock(&echoMutex);
				objectFront = 1;
				pthread_mutex_unlock(&echoMutex);
			}
			else if(sensorTrig == RIGHTECHOT){
				printf("%f, SOMETHING TO THE RIGHT\n", sensorStatus);
				pthread_mutex_lock(&echoMutex);
				objectRight = 1;
				pthread_mutex_unlock(&echoMutex);
			}

		}
		//end of each echo update
		printf(" %d, %d, %d\n", objectLeft, objectFront, objectRight);
	}
}

int main(int argc, char *argv[]){
	/*wiringpi setup*/
	if (wiringPiSetup() == -1) {
		printf("Failed to setup Wiring Pi!\n");
		return 1;
	}
	printf("wiringPi initialized\n");
	/*sensor setup*/

	pthread_t detector[3];
	for(int i = 0; i < 3; i++){
		echoSensorArgs *args = malloc(sizeof *args);
		if( i == 0 ){//center echo
			printf("Creating thread 1\n");
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
	
	for(int i = 0; i < 3; i++){
		pthread_join(detector[i], NULL);
	}

  return 0;
}
