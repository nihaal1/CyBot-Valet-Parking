// Main
/**
 *
 */

#include "timer.h"

#include "lcd.h"

#include "uart_extra_help.h"

#include "resetSimulation.h"

#include "movement.h"

#include "open_interface.h"

#include "time.h"

#include "adc.h"

#include "ping.h"

#include "servo.h"

#include "button.h"

//#include "music.h"

volatile char uart_data; // Your UART interupt code can place read data here
volatile char flag; // Your UART interupt can update this flag

char buffer[21];
typedef struct d {
  float data;
  int angle;
  int radialWidth;
  int width;
  int startAngle;
  int endAngle;
  int rawData;
  int estimatedDistance;
}
object_t;

object_t objects[10];
float data[91];
int dist[91];
int IR_dist = 0;
int startIndex = 0;
int endIndex = 0;
int currentRadialSize = 0;
int radialAngle = 0;
double otherAngle = 0;
int minSize = 0;
int minIndex = 0;
int index = 0;
int found = 0;
int notFound = 0;
int total = 0;
int measure = 0;
char str[1000];
char str1[1000];
char str2[1000];
char str3[1000];
char str4[1000];
int i;
int j;
int a;
int b;
int c;
int pulse;
char ch = 0;
char swap = 0;
int count = 0;
int parkingCount = 0;
int button = 0;
int parked = 0;
char select;
extern int stop;
int scan = 0, q, min;

void detect() {
  index = 0;
  servo_move(60);
  timer_waitMillis(500);
  for (i = 0; i < 60; i += 10) {
    ping_activate();
    q = ping_read();
    servo_move(i + 60);
    timer_waitMillis(100);

    if (q <= 40) {
      index = 1;
    }
  }

}
void recall() {
  oi_t * sensor = oi_alloc();
  oi_init(sensor);
  load_songs();
  int move = 0;
  turnRight(sensor, 175);
  collision_detection(sensor, 50);

  while (sensor -> cliffRightSignal < 2500 && sensor -> cliffLeftSignal < 2500) {
    while (move == 0) {
      min = 100;
      for (i = 30; i < 150; i += 10) {
        ping_activate();
        q = ping_read();
        servo_move(i);
        timer_waitMillis(100);
        if (q < min) {
          min = q;
        }
      }
      if (min > 50) {
        move = 1;
        oi_setLeds(1, 1, 6, 255);
        break;
      }
      oi_setLeds(1, 1, 255, 255);
            oi_play_song(1);
            oi_update(sensor);
    }
    collision_detection(sensor, 5);

  }
  collision_detection(sensor, 80);
  turnLeft(sensor, 85);
  while (sensor -> cliffLeftSignal < 2650) {
    if (sensor -> cliffFrontRightSignal > 2500 &&
      sensor -> cliffFrontLeftSignal > 2500) {
      collision_detection(sensor, 3);
      scan++;
      if (scan == 15) {
        scan = 0;
        servo_move(0);
        timer_waitMillis(500);
        for (i = 0; i < 190; i += 10) {
          ping_activate();
          q = ping_read();
          servo_move(i);
          timer_waitMillis(100);

          if (q <= 20) {
            stop = 1;
          }
        }
      }
    }
    if (sensor -> cliffFrontRightSignal <= 2500) {
      GradualturnLeft(sensor, 1);
    }
    if (sensor -> cliffFrontLeftSignal <= 2500) {
      GradualturnRight(sensor, 1);
    }
    while (stop == 1) {
      oi_setLeds(1, 1, 255, 255);
      min = 100;
      oi_play_song(1);
      oi_update(sensor);
      oi_setWheels(0, 0);
      servo_move(0);
      timer_waitMillis(3000);
      for (i = 0; i < 190; i += 10) {
        ping_activate();
        q = ping_read();
        servo_move(i);
        timer_waitMillis(100);
        if (q < min) {
          min = q;
        }
      }
      if (min > 20) {
        stop = 0;
        oi_setLeds(1, 1, 7, 255);
        break;
      }

    }
  }

  collision_detection(sensor, 150);
  oi_setLeds(0, 0, 0, 0);
  uart_sendStr("Exited parking lot\n\r");
  oi_free(sensor);
}
void park() {
  oi_t * sensor = oi_alloc();
  oi_init(sensor);
  load_songs();
  oi_setLeds(1, 1, 7, 255);
  timer_waitMillis(1000);
  collision_detection(sensor, 300);
  while (parked == 0) {
    if (sensor -> cliffFrontRightSignal > 2500 && sensor -> cliffFrontLeftSignal > 2500) {
      collision_detection(sensor, 3);
      scan++;

      if (scan == 15) {
        scan = 0;
        servo_move(0);
        timer_waitMillis(500);
        for (i = 0; i < 190; i += 10) {
          ping_activate();
          q = ping_read();
          servo_move(i);
          timer_waitMillis(100);

          if (q <= 20) {
            stop = 1;
          }
        }
      }

    }
    if (sensor -> cliffFrontRightSignal <= 2500) {
      GradualturnLeft(sensor, 1);
    }
    if (sensor -> cliffFrontLeftSignal <= 2500) {
      GradualturnRight(sensor, 1);
    }

    while (stop == 1) {
      oi_setLeds(1, 1, 255, 255);

      oi_play_song(1);
      oi_update(sensor);
      min = 100;
      oi_setWheels(0, 0);
      servo_move(0);
      timer_waitMillis(3000);
      for (i = 0; i < 190; i += 10) {
        ping_activate();
        q = ping_read();
        servo_move(i);
        timer_waitMillis(100);
        if (q < min) {
          min = q;
        }
      }
      if (min > 20) {
        stop = 0;
        oi_setLeds(1, 1, 7, 255);
        break;
      }

    }

    if (sensor -> cliffRightSignal > 2600) {
      turnLeft(sensor, 85);
      detect();
      parkingCount++;
      if (index > 0) {
        uart_sendStr("Found car parked\n\r");
        turnRight(sensor, 85);
        servo_move(0);
                       timer_waitMillis(500);
                       for (i = 0; i < 190; i += 10) {
                         ping_activate();
                         q = ping_read();
                         servo_move(i);
                         timer_waitMillis(100);

                         if (q <= 25) {
                           stop = 1;
                         }
                       }
        collision_detection(sensor, 25);

        if (parkingCount == 4) {
          while (sensor -> cliffLeftSignal < 2650) {
            if (sensor -> cliffFrontRightSignal > 2500 &&
              sensor -> cliffFrontLeftSignal > 2500) {
              collision_detection(sensor, 3);
              scan++;

              if (scan == 15) {
                scan = 0;
                servo_move(0);
                timer_waitMillis(500);
                for (i = 0; i < 190; i += 10) {
                  ping_activate();
                  q = ping_read();
                  servo_move(i);
                  timer_waitMillis(100);

                  if (q <= 20) {
                    stop = 1;
                  }
                }
              }
            }
            if (sensor -> cliffFrontRightSignal <= 2500) {
              GradualturnLeft(sensor, 1);
            }
            if (sensor -> cliffFrontLeftSignal <= 2500) {
              GradualturnRight(sensor, 1);
            }
            while (stop == 1) {
              min = 100;
              oi_setWheels(0, 0);
              servo_move(0);
              timer_waitMillis(3000);
              for (i = 0; i < 190; i += 10) {
                ping_activate();
                q = ping_read();
                servo_move(i);
                timer_waitMillis(100);
                if (q < min) {
                  min = q;
                }
              }
              if (min > 20) {
                stop = 0;
                break;
              }

            }
          }
          oi_setWheels(0, 0);
          collision_detection(sensor, 150);
          uart_sendStr("Exited parking lot, no parking spots available :(\n\r");
          parked = 1;
          oi_free(sensor);
        }
        while (sensor -> cliffRightSignal > 2500) {
          collision_detection(sensor, 5);
        }
      }
      if (index == 0) {
        while (sensor -> cliffFrontRightSignal < 2600 || sensor -> cliffFrontLeftSignal < 2600) {
          collision_detection(sensor, 5);
        }
        oi_setWheels(0, 0);
        parked = 1;
        sprintf(str4, "You parked at parking spot %d\n\r", parkingCount);
        uart_sendStr(str4);
        oi_free(sensor);
      }
    }

  }

}

void main() {

  timer_init();
  lcd_init();
  button_init();
  uart_init(115200);
  ping_initialize();
  adc_init();
  servo_init();

  char * init_main = "Options\n\rPark(p)\n\r";
  uart_sendStr(init_main);

  while (1) {

    select = uart_receive();
    if (select == 'p') {
      park();
      init_main = "Options\n\rRecall(r)\n\r";
      uart_sendStr(init_main);

    }
    if (select == 'r') {
      recall();
    }

  }

}
