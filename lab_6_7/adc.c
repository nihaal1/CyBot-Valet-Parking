/**
 * adc.c
 *
 *  Created on: Jun 10, 2022
 *      Author: vsadvani
 */


#include "adc.h"
#include <stdio.h>

void adc_init(){
//Turn on System clock for Port B
  SYSCTL_RCGCGPIO_R |= 0b000010;
 //delay for clock to start and to set up peripherals
  while((SYSCTL_PRGPIO_R & 0b000010) == 0){};
  //toggle alternate function on wire 4
  GPIO_PORTB_AFSEL_R |= 0b00010000;
  //set alternate function to input
  GPIO_PORTB_DIR_R &= 0b11101111;
  //digitally enable PB4
  GPIO_PORTB_DEN_R |= 0b00010000;
  //enable analog function of PB4
  GPIO_PORTB_AMSEL_R |= 0b00010000;
  //PB4 can be used as an external trigger therefore no wire is used to trigger ADC
  GPIO_PORTB_ADCCTL_R &= 0x00;
  //Enable and provide clock to ADC module 0 in Run Mode
  SYSCTL_RCGCADC_R |= 0b00000001;
  //delay to make sure ADC module 0 is ready for use
  while((SYSCTL_PRADC_R & 0b00000001) == 0){};
  //Enable sample sequencer 0
  ADC0_ACTSS_R |= 0b00000001;
  //Always trigger sample sequencer 0
  ADC0_EMUX_R |= 0x000F;
  //Selet 1st sample input on wire 0 and sample channel 10
  ADC0_SSMUX0_R = 0x000A;
  //Assert raw interrupt signal and end sample sequence controller after the first sample input
  ADC0_SSCTL0_R |= 0x0006;
  //64x oversampling
  ADC0_SAC_R |= 0x6;
}

int adc_read(){
 int raw_val = 0;
 //Start sampling on sample sequencer 0
  ADC0_PSSI_R |= 0b00000001;
 //wait for sample sequencer to complete conversion and enable raw interrupt
  while((ADC0_RIS_R & 0b00000001) == 0){};
  //store results of conversion data
  raw_val = ADC0_SSFIFO0_R & 0xFFF;
  //send level based trigger to interrupt handler from INR0
  ADC0_ISC_R = 0b00000001;
 return raw_val;
}

