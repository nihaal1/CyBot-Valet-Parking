/**
 * lab6_template_extra_help.c
 *
 * Description: This is file is meant for those that would like a little
 *              extra help with formatting their code.
 *
 */

#include "timer.h"

#include "lcd.h"

#include "uart_extra_help.h"

#include "resetSimulation.h"

#include "movement.h"

#include "open_interface.h"

#include "cyBot_Scan.h"

#include "time.h"

#include "adc.h"

#include "ping.h"

#include "servo.h"

#include "button.h"

// Adding global volatile varibles for communcating between 
// your Interupt Service Routine, and your non-interupt code.

volatile char uart_data; // Your UART interupt code can place read data here
volatile char flag; // Your UART interupt can update this flag
// to indicate that it has placed new data
// in uart_data

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
int i;
int j;
int a;
int b;
int c;
int pulse;
char ch = 0;
char swap = 0;
int count = 0;
int button = 0;
int parked=0;

void detect() {
  //  cyBOT_Scan_t scan;
    char * init = "Angle\tDistance(IR in cm)\tDistance(Ping in cm)\n\r";
    uart_sendStr(init);
    index = 0;
    for (i = 0; i <= 180; i += 2) {

//        servo_move(i);
//         timer_waitMillis(150);
//        pulse = pulse_width();
//        data[i / 2] = ping_read(pulse);
        ping_activate();
        data[i/2] = ping_read();
        IR_dist = (11243 * pow(adc_read(), -0.7693)) - 14;
        //IR_dist = (scan.IR_raw_val - 3351)/(-64.31);
       // dist[i / 2] = IR_dist;
        dist[i/2] = adc_read()/10;

        servo_move(i);
        timer_waitMillis(150);

        sprintf(str, "%d\t%d\t\t\t\t%.2f\n\r", i, IR_dist, data[i/2]);
        uart_sendStr(str);
    }
    for (j = 0; j <= 90; j++) {
        if (dist[j] > 85) {
            if (found == 0) {
                objects[index].startAngle = 2 * j;
                // objects[index].angle = 2*j;
                startIndex = data[j];
                //               objects[index].rawData = dist[j]*10;
                //               objects[index].estimatedDistance = (objects[index].rawData - 3351)/(-64.31);
                found++;
            }
            else if(j == 90 && found >=4){
                objects[index].endAngle = 2 * j;
                              objects[index].radialWidth = objects[index].endAngle - objects[index].startAngle;
                              objects[index].angle = 2 * (j - (found / 2));
                              endIndex = data[j];

                                              otherAngle = -2 * (double)startIndex * (double)endIndex * cos(((double)objects[index].radialWidth)*(3.14/180));
                                              objects[index].width = sqrt(2*((double)startIndex *(double)endIndex) + otherAngle);
              //                otherAngle = (180 - objects[index].radialWidth) / 2;
              //                objects[index].width = 2 * cos((double)((otherAngle) * (3.14 / 180))) * (double) objects[index].data;
                              index++;
                              found = 0;
            }
            else {
                objects[index].data = data[j];
                objects[index].endAngle = 2 * (j + found);

                found++;
            }
        }
        if (dist[j] < 85) {
            if (found >= 4) {
                objects[index].endAngle = 2 * j;
                objects[index].radialWidth = objects[index].endAngle - objects[index].startAngle;
                objects[index].angle = 2 * (j - (found / 2));
                endIndex = data[j];

                                otherAngle = -2 * (double)startIndex * (double)endIndex * cos(((double)objects[index].radialWidth)*(3.14/180));
                                objects[index].width = sqrt(2*((double)startIndex *(double)endIndex) + otherAngle);
//                otherAngle = (180 - objects[index].radialWidth) / 2;
//                objects[index].width = 2 * cos((double)((otherAngle) * (3.14 / 180))) * (double) objects[index].data;
                if( 2 * (j - (found / 2)) >= 70 &&  2 * (j - (found / 2))< 110){
                index++;
                }
                found = 0;

            }
            if (found < 4 && found > 0) {
                found = 0;
            }
        }

    }

    char str_new[1000];
    char * init_next = "\n\rObject#\tAngle\tDistance(in cm)\t\tRadial Width\t\tWidth( in cm)\n\r";
    uart_sendStr(init_next);
    int minSize = objects[0].width;
    for (c = 0; c < index; c++) {
        sprintf(str_new, "\n\r%d\t%d\t\t%d\t\t%d\t\t\t%d\n\r", c + 1, (objects[c].angle), (int) objects[c].data, objects[c].radialWidth, objects[c].width);
        uart_sendStr(str_new);
        if (objects[c].width < minSize) {
            minSize = objects[c].width;
            minIndex = c;
        }
    }
   // cyBOT_Scan(0, & scan);
    servo_move(0);
    //cyBOT_Scan(objects[minIndex].angle, & scan);
    servo_move(objects[minIndex].angle);
    //index = 0;
}
void main() {

    timer_init();
    lcd_init();
    button_init();
    oi_t * sensor = oi_alloc();
    oi_init(sensor);
    uart_init(115200);
   ping_initialize();
    adc_init();
    servo_init();

    right_calibration_value = 327250;
    left_calibration_value = 1293250;
    //Part 3
    //      uart_interrupt_init();
    //      while(1){
    //
    //  }
    //Part 1 and Part 2
    //      while(1){
    //          uart_sendStr("Please input a character");
    //          buffer[20] = '\0';
    //          for( i = 0; i<20;i++){
    //              ch = uart_receive();
    //              uart_sendChar(ch);
    //              if(ch == '\r'){
    //                  uart_sendChar('\n');
    //                  buffer[i] = '\0';
    //                  break;
    //              }
    //              buffer[i] = ch;
    //              lcd_printf("Character %d is %c", i+1, ch);
    //          }
    //          if(ch != '\r'){
    //              uart_sendStr("\r\n");
    //          }
    //          lcd_printf(buffer);
    //      }

    //Part 4

    while(parked==0){
                            sprintf(str, "%d", sensor->cliffFrontLeftSignal );
                            sprintf(str1, "%d", sensor->cliffFrontRightSignal );
                            sprintf(str2, "%d", sensor->cliffRightSignal );
                            lcd_printf("%s    %s \n   %s",str,str1,str2);



        if(sensor->cliffFrontRightSignal>2500 && sensor->cliffFrontLeftSignal>2500){
            moveForward(sensor, 5);
        }
        if(sensor->cliffFrontRightSignal<2500){
                   GradualturnLeft(sensor, 3);
                }
        if(sensor->cliffFrontLeftSignal<2500){
            GradualturnRight(sensor, 3);
                       }

//        if(sensor->cliffFrontRightSignal<2550 && sensor->cliffFrontLeftSignal<2550){
//            oi_setWheels(0, 0);
//            break;
//        }
        if(sensor->cliffRightSignal>2500){
            turnLeft(sensor, 85);
            detect();
            if(index>0){
            turnRight(sensor, 85);
            while(sensor->cliffRightSignal>2500){
                moveForward(sensor, 5);
            }
            }
            if(index==0){
                while(sensor->cliffFrontRightSignal<2500 && sensor->cliffFrontLeftSignal<2500){
                    moveForward(sensor, 5);
                }
                oi_setWheels(0, 0);
               parked=1;
            }
        }



    }


//    while (1) {
////        swap = uart_receive();
////        if (swap == 't')
////        {
////            button = button_getButton();
////            if (button == 4)
////            {
////           lcd_printf("scammmmmmmmmmmm");
////                moveForward(sensor, 50);
////            }
//            count = 0;
////            sprintf(str, "%d", sensor->cliffFrontLeftSignal);
////            lcd_printf(str);
//            while (1) {
//                ch = uart_receive();
//                if (ch == 'w') {
//                   // collision_detection(sensor, 200);
//                   // detect();
//                    moveForward(sensor, 50);
//                    sprintf(str, "%d", sensor->cliffFrontLeftSignal );
//                    sprintf(str1, "%d", sensor->cliffFrontRightSignal );
//                    sprintf(str2, "%d", sensor->cliffLeftSignal );
//                    sprintf(str3, "%d", sensor->cliffRightSignal );
//                    lcd_printf("%s    %s\n     %s    %s\n",str,str1,str2,str3);
////                    if(sensor->cliffFrontLeft == 1 || sensor->cliffFrontRight == 1 || sensor->cliffLeft == 1 || sensor->cliffRight == 1){
////                            moveBackward(sensor, 500);
//
//
////                        }
//
//                } else if (ch == 's') {
//                    moveBackward(sensor, 50);
//                    sprintf(str, "%d", sensor->cliffFrontLeftSignal);
//                    sprintf(str1, "%d", sensor->cliffFrontRightSignal);
//                    sprintf(str2, "%d", sensor->cliffLeftSignal);
//                    sprintf(str3, "%d", sensor->cliffRightSignal);
//                    lcd_printf("%s    %s\n     %s    %s\n", str, str1, str2, wstr3);
//                }
//                else if (ch == 'a')
//                {
//                    turnLeft(sensor, 2);
//                } else if (ch == 'd') {
//                    turnRight(sensor, 2);
//                } else if (ch == 'm') {
//                    detect();
//
//                } else {
//                    break;
//                }
//
//            }



//        } else if (swap == 'j') {
//            lcd_printf("autonomous");
//            detect();
//            count++;
//
//            if (objects[minIndex].angle < 90) {
//
//                turnRight(sensor, 82);
//                turnLeft(sensor, objects[minIndex].startAngle);
//                collision_detection(sensor, (objects[minIndex].data * 10) - 125);
//            } else if (objects[minIndex].angle > 90) {
//
//                turnLeft(sensor, 82);
//                turnRight(sensor, 180 - objects[minIndex].startAngle);
//                collision_detection(sensor, (objects[minIndex].data * 10) - 125);
//            }
//
////            if(index == 1){
////                moveBackward(sensor, 150);
////                if(objects[minIndex].angle > 90){
////                    turnRight(sensor, 82);
////                    moveForward(sensor, 200);
//////                    turnLeft(sensor, 82);
//////                    moveForward(sensor, 200);
////                }
////                else if(objects[minIndex].angle < 90){
////                    turnLeft(sensor,  82);
////                    moveForward(sensor, 200);
////                }
////            }
//            }
//        else if(count == 1){
//           // count = 0;
//            break;
//        }
//


    oi_free(sensor);
}
