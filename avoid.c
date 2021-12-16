#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include "echo.h"
#include "motor.h"
#include "lineSensor.h"

avoidancestart(){
printf("/%d,%d,%d Avoidance routine sensor value \n",objectLeft,objectFront,objectRight);
    if(objectFront == 1){
        turn(1,50);
        //start the 1st left turn until middle echo sensor no longer
        //sees the object
        while(objectFront == 1){
        }
        stop();
        //need to add turn for one second to avoid object corners

        //else if statements for cases either where leftsensor sees  the
        //object or turned too much and no longer sees it, in which case it will
        //see it again as the car passes the object
        if(objectRight == 0){
            forward(50,1);// 1st case to wide of a turn
            while(objectRight == 0){
                }//stops when object is detected again
            stop()
            forward(speed,1);
            while(objectRight== 1){
                }
            stop();//reaches the end of the object
            //add time based 1 second
        }
        else if(objectRight == 1){
            forward(speed,1);// 2nd case still sees object
            while(objectRight == 1){
                }
            stop();//stops when it no longer sees it
            //add time based forward
            }


        //now car should be past object time to turn right
        turn(0,50);
        while(objectRight == 0){
        }       
        stop();//picks up the object
        turn(0,50);//continues to turn
        while(objectRight == 1){
        }
        stop();//stops when right sensor no longer see it
        forward(50,1);//moves towards the line
        while(middleLineSensor == 0){
        }
        stop();//stops when middle line sensor catches the line
        turn(1,50);//turns left to straighten out, so until left line sensor doesn't see it
        while(leftLineSensor == 0){
        }
        stop();//stops and gives control back to routeDecicer
    }
}