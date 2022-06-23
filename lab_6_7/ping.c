/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include <math.h>
#include "ping.h"

#include "Timer.h"

#include "driverlib/interrupt.h"

#include "lcd.h"
volatile unsigned int startPeriod = 0;
volatile unsigned int endPeriod = 0;
volatile int clockCyclePeriod;
volatile int clockCycleCount = 0;
volatile int edge = 0;
volatile int interrupt_control = 0;
volatile int overflow = 0;

void ping_initialize(void) {
    //Start system clock for port B
      SYSCTL_RCGCGPIO_R |= 0X2;
    //Start system clock for timer 3
      SYSCTL_RCGCTIMER_R |= 0X8;
    //Setup peripherals
      while ((SYSCTL_PRGPIO_R & 0x2) == 0) {};
      while ((SYSCTL_PRTIMER_R & 0x8) == 0) {};
    //Digitally enable PB3 to input for input capture
      GPIO_PORTB_DEN_R |= 0X8;
      GPIO_PORTB_DIR_R &= 0xF7;
      //Activate alternate function of PB3
      GPIO_PORTB_AFSEL_R |= 0x8;
      //Clear the alternate function sent to wire PB3
      GPIO_PORTB_PCTL_R &= 0xFFFF0FFF;
      //Set the alternate function set on PB3 to T3CPP1
      GPIO_PORTB_PCTL_R |= 0x00007000;
      //Disable Timer B
      TIMER3_CTL_R &= ~0x100;
      //Configure to 16/32 timer configuration
      TIMER3_CFG_R = 0x4;
      //Edge-time capture mode
      TIMER3_TBMR_R |= 0x7;
      //Count down
      TIMER3_TBMR_R &= ~0x10;
      //Enable both edges for Timer B event
      TIMER3_CTL_R |= 0xC00; //0xD00
     //for 16/32 entire 8 bit prescaler
      TIMER3_TBPR_R = 0xFF;
     //Interval load register for 16/32 bit register
      TIMER3_TBILR_R = 0xFFFF;
    //Enable interrupt for timer b capture mode event
      TIMER3_IMR_R |= 0x400;
      //Clear interrupt for timer b capture mode event
      TIMER3_ICR_R |= 0x400;
      //enable timer b
      TIMER3_CTL_R |= 0x100;
    //Interrupt number 36
      NVIC_EN1_R |= 0x10;
      //IRQ 21
      NVIC_PRI9_R = (NVIC_PRI9_R & 0xFFFFFF0F) | 0x00000020;
     // NVIC_EN1_R |= 0x10;
      IntRegister(INT_TIMER3B, timer_handler);
      IntMasterEnable();
      //Enable timer B after interrupt
      TIMER3_CTL_R |= 0x100;


}

void ping_activate(void) {
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0x400;
  //Clear alternate function on PB3
    GPIO_PORTB_AFSEL_R &= ~0x8;
  //Set PB3 as output
    GPIO_PORTB_DIR_R |= 0x08;
    //Clear data on PB3
    GPIO_PORTB_DATA_R &= ~0x08;
    //Set data on PPB3
    GPIO_PORTB_DATA_R |= 0x08;
    //wait at least 5 micro seconds as recommended on data sheet
    timer_waitMicros(5);
    //clear data o wire PB3
    GPIO_PORTB_DATA_R &= ~0x08;
  //clear interrupt for timer b capture event
    TIMER3_ICR_R |= 0x400;
  //enable alternate function on PB3
    GPIO_PORTB_AFSEL_R |= 0x8;
    //Enable interrupt for timer b capture event
    TIMER3_IMR_R |= 0x400;
    //enable timer b
    TIMER3_CTL_R |= 0x100;
}

void timer_handler(void) {

    //clear interrupt
 TIMER3_ICR_R |= 0x400;
 //rising edge detected
 if(edge == 0){
     startPeriod = TIMER3_TBR_R;
     edge = 1;
     interrupt_control = 0;
 }
 //falling edge detected
 else{
     endPeriod = TIMER3_TBR_R;
     edge = 0;
     interrupt_control = 1;

 }

}

float ping_read(void) {
   // ping_activate();
    //wait until falling edge is detected
    while(interrupt_control == 0){}
    //store clock cycle

    clockCyclePeriod = startPeriod - endPeriod;
    //if overflow detected
    if(clockCyclePeriod < 0){
        //reset clock cycle to boundary of 2^24
        clockCyclePeriod += pow(2, 24);
        overflow += 1;
    }
    interrupt_control = 0;
    //one clock cycle has time period 1/32000000

    return clockCyclePeriod * 6.25e-8 * 34300 / 2;

}
