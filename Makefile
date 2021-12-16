CC=gcc
CFLAGS=-Wall
LIBS= -lwiringPi -lpthread -lm
EXECUTABLE= main

.DEFAULT_GOAL := main

main: main.c
	sudo $(CC) $(CFLAGS) -o $(EXECUTABLE) main.c DEV_Config.c PCA9685.c motor.c lineSensor.c avoidanceSensor.c echo.c $(LIBS)

clean:
	@rm -f $(EXECUTABLE)

run: main
	$(CC) $(CFLAGS) -o $(EXECUTABLE) main.c DEV_Config.c PCA9685.c motor.c lineSensor.c avoidanceSensor.c echo.c $(LIBS)
	./main
