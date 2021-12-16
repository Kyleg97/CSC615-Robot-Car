#include "echo.h"
/*
//Left Echo
#define LEFTECHOT 8
#define LEFTECHOE 9
//Center Echo
#define CENTERECHOT 15
#define CENTERECHOE 16
//Right Echo
#define RIGHTECHOT 12
#define RIGHTECHOE 13
*/
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

	printf("%d, %d\n",sensorTrig, sensorEcho);
	pinMode(sensorTrig, OUTPUT);
	digitalWrite(sensorTrig, LOW);
	pinMode(sensorEcho, INPUT);


	while(counter < 5){
		delay(5000);//delay from over printing
		digitalWrite(sensorTrig, HIGH);
		delay(0.1);
		digitalWrite(sensorTrig, LOW);

		int firstEcho, secondEcho, low, high;
		firstEcho = secondEcho = low = high = 0;
		
		while(low == 0 || high == 0){ //waits to store both values begin and end to calculate distance
			printf("inwhileloop");
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

