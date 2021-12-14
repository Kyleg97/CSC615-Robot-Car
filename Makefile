all: main.c
	sudo gcc -Wall -o main main.c DEV_Config.c PCA9685.c motor.c lineSensor.c avoidanceSensor.c -lm -lwiringPi -lpthread

main: main.c
	sudo gcc -Wall -o main main.c DEV_Config.c PCA9685.c motor.c lineSensor.c avoidanceSensor.c -lm -lwiringPi -lpthread

clean:
	sudo rm main

run:
	sudo gcc -Wall -o main main.c DEV_Config.c PCA9685.c motor.c lineSensor.c avoidanceSensor.c -lm -lwiringPi -lpthread
	sudo ./main
