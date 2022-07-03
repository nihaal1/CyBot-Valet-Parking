#include "movement.h"
#include "open_interface.h"
#include "time.h"
#include "servo.h"
#include "ping.h"
#include "adc.h"
#include "button.h"
#include "uart_extra_help.h"

int i, stop=0, button_number=0;



void moveForward(oi_t *sensor, double dist_mm){
    double sum = 0;
    //both wheels at max speed in the forward direction
    oi_setWheels(100 , 100);
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
    oi_setWheels(-100, -100);
    //Negative values in backward direction
    while(sum > dist_mm){
        oi_update(sensor);
        sum += sensor ->distance;
    }
    oi_setWheels(0, 0);//Stop
    //oi_free(sensor);
}
void GradualturnRight(oi_t *sensor, double dist_mm){


    //sets the wheel
    double sum = 0;
    oi_setWheels(20, 100);
    // a while loop to make it move
    while(sum < dist_mm){
        oi_update(sensor);
        //random number for when it bumps in the middle

        // when it bumps into the middle sensor chooses between left or right turn
        if(sensor->bumpLeft == 1 || sensor->bumpRight == 1 || (sensor->bumpLeft == 1 && sensor->bumpRight == 1 )){
            button_number=0;
            oi_setWheels(0, 0);
            moveBackward(sensor,100);
            uart_sendStr("Small object found. Parking attendant notified.");
            lcd_printf("press button 4 to\n resume motion once\n object is cleared.");
            oi_setLeds(1, 1, 255, 255);
            while(button_number!=4){
                button_number=button_getButton();
            }
            lcd_clear();
            uart_sendStr("Small object cleared. Vehicle motion resumed.");
            oi_setWheels(100, 100);



        }

        sum += sensor ->distance;
    }
    oi_setWheels(0, 0);

}

void GradualturnLeft(oi_t *sensor, double dist_mm){


    //sets the wheel
    double sum = 0;
    oi_setWheels(100, 20);
    // a while loop to make it move
    while(sum < dist_mm){
        oi_update(sensor);
        //random number for when it bumps in the middle

        // when it bumps into the middle sensor chooses between left or right turn
        if(sensor->bumpLeft == 1 || sensor->bumpRight == 1 || (sensor->bumpLeft == 1 && sensor->bumpRight == 1 )){
            button_number=0;
            oi_setWheels(0, 0);
            moveBackward(sensor,100);
            uart_sendStr("Small object found. Parking attendant notified.");
            lcd_printf("press button 4 to\n resume motion once\n object is cleared.");
            oi_setLeds(1, 1, 255, 255);
            while(button_number!=4){
                button_number=button_getButton();
            }
            lcd_clear();
            uart_sendStr("Small object cleared. Vehicle motion resumed.");
            oi_setWheels(100, 100);



        }

        sum += sensor ->distance;
    }
    oi_setWheels(0, 0);

}



void turnRight(oi_t *sensor, double angle){
    double currentAngle = 360;
    //clockwise angle
    angle = 360 - angle;
    //right wheel at half the max speed in the backward direction
    //left wheel at half the max speed in the forward direction
    oi_setWheels(-50, 50);
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
    oi_setWheels(50, -50);
    //counter clockwise angles are always positive
    while(currentAngle < angles){
        oi_update(sensor);
        currentAngle += sensor ->angle;
    }
    oi_setWheels(0, 0);//Stop
    // oi_free(sensor);
}
void load_songs() {

    // Notes: oi_load_song takes four arguments
    // arg1 - an integer from 0 to 16 identifying this song
    // arg2 - an integer that indicates the number of notes in the song (if greater than 16, it will consume the next song index's storage space)
    // arg3 - an array of integers representing the midi note values (example: 60 = C, 61 = C sharp)
    // arg4 - an array of integers representing the duration of each note (in 1/64ths of a second)
    //
    // For a note sheet, see page 12 of the iRobot Creat Open Interface datasheet

    unsigned char rickrollNumNotes = 1;
    unsigned char rickrollNotes[1]    = {65};
    unsigned char rickrollDuration[1] = {64};
    oi_loadSong(1, rickrollNumNotes, rickrollNotes, rickrollDuration);

    unsigned char ImperialMarchNumNotes = 1;
    unsigned char ImperialMarchNotes[1]     = {55};
    unsigned char ImperialMarchDurations[1] = {32};
    oi_loadSong(2, ImperialMarchNumNotes, ImperialMarchNotes, ImperialMarchDurations);

    unsigned char mario1NumNotes = 49;
    unsigned char mario1Notes[49]    = {48, 60, 45, 57, 46, 58,  0, 48, 60, 45, 57, 46, 58,  0, 41, 53, 38, 50, 39, 51,  0, 41, 53, 38, 50, 39, 51,  0, 51, 50, 49, 48, 51, 50, 44, 43, 49, 48, 54, 53, 52, 58, 57, 56, 51, 47, 46, 45, 44 };
    unsigned char mario1Duration[49] = {12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 62, 12, 12, 12, 12, 12, 12, 48,  8,  8,  8, 24, 24, 24, 24, 24, 24,  8,  8,  8,  8,  8,  8, 16, 16, 16, 16, 16, 16 };
    oi_loadSong(3, mario1NumNotes, mario1Notes, mario1Duration);

    unsigned char mario2NumNotes = 65;
    unsigned char mario2Notes[65]    = {50, 52, 54, 55, 57, 58, 59, 59, 59, 59, 59, 55, 64, 63, 64, 0, 55, 57, 59, 60, 62, 64, 63, 65, 64,  0, 55, 62, 61, 62,  0, 55, 57, 59, 60, 61, 62, 55, 65, 64,  0, 55, 67, 67, 67, 67, 69,  0, 67, 65, 65, 65, 65, 67,  0, 65, 64, 57, 59, 65, 64, 64, 64, 59, 60 };
    unsigned char mario2Duration[65] = {18, 18, 18, 18, 18, 18,  9,  9, 18, 18, 45,  9, 54, 54, 54, 9,  9,  9,  9,  9,  9, 54, 36, 18, 54, 45,  9, 54, 54, 54,  9,  9,  9,  9,  9,  9, 54, 36, 18, 54, 45,  9, 54, 54, 54, 18, 18,  9,  9, 54, 54, 54, 18, 18,  9,  9, 54, 18, 18, 18,  9,  9, 27,  9, 54 };
    oi_loadSong(4, mario2NumNotes, mario2Notes, mario2Duration);
}

void collision_detection(oi_t *sensor, double dist_mm){


    //sets the wheel
    double sum = 0;
    oi_setWheels(100, 100);
    // a while loop to make it move
    while(sum < dist_mm){
        oi_update(sensor);
        //random number for when it bumps in the middle

        // when it bumps into the middle sensor chooses between left or right turn
        if(sensor->bumpLeft == 1 || sensor->bumpRight == 1 || (sensor->bumpLeft == 1 && sensor->bumpRight == 1 )){
            button_number=0;
            oi_setWheels(0, 0);
            moveBackward(sensor,100);
            uart_sendStr("Small object found. Parking attendant notified.");
            lcd_printf("press button 4 to\n resume motion once\n object is cleared.");
            oi_setLeds(1, 1, 255, 255);
            load_songs();
            oi_play_song(2);
            while(button_number!=4){
                button_number=button_getButton();
                oi_play_song(2);
                oi_t * sensor = oi_alloc();
            }
            lcd_clear();
            oi_setLeds(1, 1, 7, 255);
            uart_sendStr("Small object cleared. Vehicle motion resumed.");
            oi_setWheels(100, 100);



        }

        sum += sensor ->distance;
    }
    oi_setWheels(0, 0);

}






