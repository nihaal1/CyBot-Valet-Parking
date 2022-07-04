/*
 * servo.c
 *
 *  Created on: Jun 20, 2022
 *      Author: vsadvani
 */
#include "servo.h"
#include "Timer.h"

#include "driverlib/interrupt.h"

#include "lcd.h"
#include "button.h"
//    20ms/62.5ns
volatile int period = 320000;

void servo_init (void){
    SYSCTL_RCGCGPIO_R |= 0b10;  // enable clock on PORT B of GPIO
    SYSCTL_RCGCTIMER_R |= 0b10; // enable clock for timer 1
    while ((SYSCTL_PRGPIO_R & 0b10) == 0) {};    // wait for ready on PORT B
    GPIO_PORTB_DIR_R |= 0b100000;    // set PB5 to output
    GPIO_PORTB_DEN_R |= 0b100000;    // enable digital output on PB5

    GPIO_PORTB_AFSEL_R |= 0b100000; // enable alt function on PB5
    GPIO_PORTB_PCTL_R &= 0xFF7FFFFF;  // set alt function as T3CCP1 function
    GPIO_PORTB_PCTL_R |= 0x700000;  // ... on PB3 (set bits [15..12] to 7) // correct to PB5


    TIMER1_CTL_R &= ~0x100; // disable Timer during startup
    TIMER1_CFG_R = 0x4;     // using 16bit with prescaler
    TIMER1_TBMR_R |= 0b1010;
    TIMER1_TBMR_R &= ~0b0101; // configure Timer B Mode for PWM one-shot
    TIMER1_CTL_R &= ~0x4000; //noniverted

    TIMER1_TBILR_R = period;

    TIMER1_TBPR_R = 0x4;
    TIMER1_TBMATCHR_R = 0xC000;
    TIMER1_TBPMR_R = 0x4;

    TIMER1_CTL_R |= 0x100; // re-enable Timer B
}
void servo_move(float degrees){
    unsigned int cycles;
    //Each degrees moves as the 16MHz clock frequency
    //16000000 * (1/180
    unsigned int pulseWidth = 160 * degrees;
    TIMER1_TBMATCHR_R = 0xC000;
    //TIMER1_TBPMR_R = 0x4;

    cycles = (TIMER1_TBPMR_R << 16) + TIMER1_TBMATCHR_R - (pulseWidth);
    TIMER1_TBMATCHR_R = cycles;
    TIMER1_TBPMR_R = cycles >> 16;

}
void servo_calibrate(){

}



