/*
 * movement.c
 *
 *  Created on: May 21, 2022
 *      Author: Varun Advani
 */

#include "movement.h"
#include "open_interface.h"
#include "time.h"
void moveForward(oi_t *sensor, double dist_mm){
    double sum = 0;
    //both wheels at max speed in the forward direction
     oi_setWheels(150 , 150);
     //Positive values in forward direction
     while(sum < dist_mm){
         oi_update(sensor);
         sum += sensor ->distance;
     }
     oi_setWheels(0, 0);//Stop
     //oi_free(sensor);
}
void moveBackward(oi_t *sensor, double dist_mm){
    //distance set to negative to calculate the sum as the iRobot moves backwards
    dist_mm *= -1;
    double sum = 0;
    //Both wheels at max speed in the backward direction
    oi_setWheels(-150, -150);
    //Negative values in backward direction
    while(sum > dist_mm){
        oi_update(sensor);
        sum += sensor ->distance;
    }
    oi_setWheels(0, 0);//Stop
    //oi_free(sensor);
}

void turnRight(oi_t *sensor, double angle){
    double currentAngle = 360;
    //clockwise angle
    angle = 360 - angle;
    //right wheel at half the max speed in the backward direction
    //left wheel at half the max speed in the forward direction
    oi_setWheels(-150, 150);
    //clokckwise angles are negative
    while(currentAngle > angle){
        oi_update(sensor);
        currentAngle += sensor->angle;

    }
    oi_setWheels(0, 0);//Stop
   // oi_free(sensor);
}

void turnLeft(oi_t *sensor, double angles){
    //counter clockwise angle
   double currentAngle = 0;
   //left wheel at half the max speed in the backward direction
   //right wheel at half the max speed in the forward direction
   oi_setWheels(150, -150);
   //counter clockwise angles are always positive
   while(currentAngle < angles){
       oi_update(sensor);
       currentAngle += sensor ->angle;
   }
   oi_setWheels(0, 0);//Stop
     // oi_free(sensor);
}
void collision_detection(oi_t *sensor, double dist_mm){


//    //sets the wheel
//    double sum = 0;
//    oi_setWheels(150, 150);
//    //to create a random numberssssssssssss
//    srand(time(0));
//    // a while loop to make it move
//    while(sum < dist_mm){
//        oi_update(sensor);
//        //random number for when it bumps in the middle
//        int r = rand()%2;
//        // when it bumps into the middle sensor chooses between left or right turn
//        if(sensor->bumpLeft == 1 && sensor->bumpRight == 1){
//            oi_setWheels(0, 0);
//            moveBackward(sensor, 150);
//            sum -= 150;
//            if(r == 0){
//                turnRight(sensor, 82);
//                moveForward(sensor, 100);
//                turnLeft(sensor, 82);
//                oi_setWheels(150, 150);
//            }
//            else{
//                turnLeft(sensor, 82);
//                moveForward(sensor, 100);
//                turnRight(sensor, 82);
//                oi_setWheels(150, 150);
//            }
//
//        }
//        //if the left sensor is bumped turns right
//        if(sensor->bumpLeft == 1){
//            oi_setWheels(0, 0);
//            moveBackward(sensor, 150);
//            sum -= 150.0;
//            turnRight(sensor, 82);
//            moveForward(sensor, 100);
//            turnLeft(sensor, 82);
//            oi_setWheels(150, 150);
//
//
//        }
//        //if the right sensor is bumped turns left
//        if(sensor->bumpRight == 1){
//            oi_setWheels(0, 0);
//            moveBackward(sensor, 150);
//            sum -= 150;
//            turnLeft(sensor, 82);
//            moveForward(sensor, 100);
//            turnRight(sensor, 82);
//            oi_setWheels(150, 150);
//
//
//        }
//        sum += sensor ->distance;
//    }
    oi_setWheels(0, 0);
}



