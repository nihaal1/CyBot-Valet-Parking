#include "movement.h"

#include "open_interface.h"

#include "time.h"

#include "servo.h"

#include "ping.h"

#include "adc.h"

#include "button.h"

#include "uart_extra_help.h"

int i, stop = 0, button_number = 0;

void moveForward(oi_t * sensor, double dist_mm) {
  double sum = 0;
  //both wheels at max speed in the forward direction
  oi_setWheels(100, 100);
  //Positive values in forward direction
  while (sum < dist_mm) {
    oi_update(sensor);
    sum += sensor -> distance;
  }
  oi_setWheels(0, 0); //Stop
  //oi_free(sensor);
}
void moveBackward(oi_t * sensor, double dist_mm) {
  //distance set to negative to calculate the sum as the iRobot moves backwards
  dist_mm *= -1;
  double sum = 0;
  //Both wheels at max speed in the backward direction
  oi_setWheels(-100, -100);
  //Negative values in backward direction
  while (sum > dist_mm) {
    oi_update(sensor);
    sum += sensor -> distance;
  }
  oi_setWheels(0, 0); //Stop
  //oi_free(sensor);
}
void GradualturnRight(oi_t * sensor, double dist_mm) {

  //sets the wheel
  double sum = 0;
  oi_setWheels(20, 100);
  // a while loop to make it move
  while (sum < dist_mm) {
    oi_update(sensor);
    //random number for when it bumps in the middle

    // when it bumps into the middle sensor chooses between left or right turn
    if (sensor -> bumpLeft == 1 || sensor -> bumpRight == 1 || (sensor -> bumpLeft == 1 && sensor -> bumpRight == 1)) {
      button_number = 0;
      oi_setWheels(0, 0);
      moveBackward(sensor, 100);
      uart_sendStr("Small object found. Parking attendant notified.");
      lcd_printf("press button 4 to\n resume motion once\n object is cleared.");
      oi_setLeds(1, 1, 255, 255);
      while (button_number != 4) {
        button_number = button_getButton();
      }
      lcd_clear();
      uart_sendStr("Small object cleared. Vehicle motion resumed.");
      oi_setWheels(100, 100);

    }

    sum += sensor -> distance;
  }
  oi_setWheels(0, 0);

}

void GradualturnLeft(oi_t * sensor, double dist_mm) {

  //sets the wheel
  double sum = 0;
  oi_setWheels(100, 20);
  // a while loop to make it move
  while (sum < dist_mm) {
    oi_update(sensor);
    //random number for when it bumps in the middle

    // when it bumps into the middle sensor chooses between left or right turn
    if (sensor -> bumpLeft == 1 || sensor -> bumpRight == 1 || (sensor -> bumpLeft == 1 && sensor -> bumpRight == 1)) {
      button_number = 0;
      oi_setWheels(0, 0);
      moveBackward(sensor, 100);
      uart_sendStr("Small object found. Parking attendant notified.");
      lcd_printf("press button 4 to\n resume motion once\n object is cleared.");
      oi_setLeds(1, 1, 255, 255);
      while (button_number != 4) {
        button_number = button_getButton();
      }
      lcd_clear();
      uart_sendStr("Small object cleared. Vehicle motion resumed.");
      oi_setWheels(100, 100);

    }

    sum += sensor -> distance;
  }
  oi_setWheels(0, 0);

}

void turnRight(oi_t * sensor, double angle) {
  double currentAngle = 360;
  //clockwise angle
  angle = 360 - angle;
  //right wheel at half the max speed in the backward direction
  //left wheel at half the max speed in the forward direction
  oi_setWheels(-50, 50);
  //clokckwise angles are negative
  while (currentAngle > angle) {
    oi_update(sensor);
    currentAngle += sensor -> angle;

  }
  oi_setWheels(0, 0); //Stop
  // oi_free(sensor);
}

void turnLeft(oi_t * sensor, double angles) {
  //counter clockwise angle
  double currentAngle = 0;
  //left wheel at half the max speed in the backward direction
  //right wheel at half the max speed in the forward direction
  oi_setWheels(50, -50);
  //counter clockwise angles are always positive
  while (currentAngle < angles) {
    oi_update(sensor);
    currentAngle += sensor -> angle;
  }
  oi_setWheels(0, 0); //Stop
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
  unsigned char rickrollNotes[1] = {
    65
  };
  unsigned char rickrollDuration[1] = {
    64
  };
  oi_loadSong(1, rickrollNumNotes, rickrollNotes, rickrollDuration);

  unsigned char ImperialMarchNumNotes = 1;
  unsigned char ImperialMarchNotes[1] = {
    55
  };
  unsigned char ImperialMarchDurations[1] = {
    32
  };
  oi_loadSong(2, ImperialMarchNumNotes, ImperialMarchNotes, ImperialMarchDurations);
}

void collision_detection(oi_t * sensor, double dist_mm) {

  //sets the wheel
  double sum = 0;
  oi_setWheels(100, 100);
  // a while loop to make it move
  while (sum < dist_mm) {
    oi_update(sensor);
    //random number for when it bumps in the middle

    // when it bumps into the middle sensor chooses between left or right turn
    if (sensor -> bumpLeft == 1 || sensor -> bumpRight == 1 || (sensor -> bumpLeft == 1 && sensor -> bumpRight == 1)) {
      button_number = 0;
      oi_setWheels(0, 0);
      moveBackward(sensor, 50);
      oi_play_song(2);
      uart_sendStr("Small object found. Parking attendant notified.");
      lcd_printf("press button 4 to\n resume motion once\n object is cleared.");
      oi_setLeds(1, 1, 255, 255);
      oi_update(sensor);
      while (button_number != 4) {
        button_number = button_getButton();
        oi_play_song(2);
        oi_t * sensor = oi_alloc();
      }
      lcd_printf("");
      oi_setLeds(1, 1, 7, 255);
      uart_sendStr("Small object cleared. Vehicle motion resumed.");
      oi_setWheels(100, 100);

    }

    sum += sensor -> distance;
  }
  oi_setWheels(0, 0);

}
