#include "motor.h"

void turn(int dir, int speed) {
    if (dir == 0) { // 0 for left turn and 1 for right
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, speed);
        PCA9685_SetLevel(PCA_CHANNEL_1, 1);
        PCA9685_SetLevel(PCA_CHANNEL_2, 0);

        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, speed);
        PCA9685_SetLevel(PCA_CHANNEL_3, 1);
        PCA9685_SetLevel(PCA_CHANNEL_4, 0);

    } else if (dir == 1){
        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, speed);
        PCA9685_SetLevel(PCA_CHANNEL_1, 1);
        PCA9685_SetLevel(PCA_CHANNEL_2, 0);

        PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, speed);
        PCA9685_SetLevel(PCA_CHANNEL_3, 1);
        PCA9685_SetLevel(PCA_CHANNEL_4, 0);
    }
}

// stops the motor
void stop() {
    // printf("Stopping motor...\n");
    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, 0);
    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, 0);
    // sleep(2); // wait 2 seconds
}

void forward(int speed, int motor) {
    // printf("Running forwards...\n");
    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, speed);
    PCA9685_SetLevel(PCA_CHANNEL_1, 1);
    PCA9685_SetLevel(PCA_CHANNEL_2, 0);

    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_5, speed);
    PCA9685_SetLevel(PCA_CHANNEL_3, 0);
    PCA9685_SetLevel(PCA_CHANNEL_4, 1);
    /*while(1) {
      if (leftLineSensor == 0) {
        printf("we need to turn right");
      }
      if (middleLineSensor == 0) {
        printf("we're in the middle");
      }
      if (rightLineSensor == 0) {
        printf("we need to turn left");
      }
    }*/
}

// 0 is left motor, 0 is right motor
void backward(int speed, int motor) {
    // printf("Running backwards...\n");
    PCA9685_SetPwmDutyCycle(PCA_CHANNEL_0, speed);
    PCA9685_SetLevel(PCA_CHANNEL_1, 1);
    PCA9685_SetLevel(PCA_CHANNEL_2, 0);
}
