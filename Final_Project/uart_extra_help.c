/*
*
*   uart_extra_help.c
* Description: This is file is meant for those that would like a little
*              extra help with formatting their code, and followig the Datasheet.
*/

#include "uart_extra_help.h"
#include "timer.h"
#include "string.h"
#include "lcd.h"
#define REPLACE_ME 0x00


void uart_init(int baud)
{
    SYSCTL_RCGCGPIO_R |= 0b00000010;      // enable clock GPIOB (page 340)
    SYSCTL_RCGCUART_R |= 0b00000010;      // enable clock UART1 (page 344)
    GPIO_PORTB_AFSEL_R |= 0b00000011;      // sets PB0 and PB1 as peripherals (page 671)
    GPIO_PORTB_PCTL_R  = 0x00000011;       // pmc0 and pmc1       (page 688)  also refer to page 650
    GPIO_PORTB_DEN_R  |= 0b00000011;        // enables pb0 and pb1
    GPIO_PORTB_DIR_R   |= 0b00000001;        // sets pb0 as output, pb1 as input

    //compute baud values [UART clock= 16 MHz]
    //Page 903 mentions a double value for the broad rate divisor
    //and an int value for fbrd
    double brd;
    int   ibrd;
    int fbrd;
    //16MHZ = 16000000
    brd = 16000000.0/(16 * baud); // page 903
    ibrd = (int)brd;
    fbrd = (int)(((brd - ibrd) * 64) + 0.5);
    // Preserve value of reserved bit 13:12
    //Disable UART 1, bit 0
    UART1_CTL_R &= 0b1100111111111110;      // disable UART1 (page 918)
    UART1_IBRD_R = ibrd;        // write integer portion of BRD to IBRD
    UART1_FBRD_R = fbrd;   // write fractional portion of BRD to FBRD
    //bits transmitted is represented by bits 6:5
    //No parity therefore bit 1 is preserved
    UART1_LCRH_R = 0b01100000;        // write serial communication parameters (page 916) * 8bit and no parity
    //system clock therefore bits 3:0 are set to 0x0
    UART1_CC_R   = 0b0000;          // use system clock as clock source (page 939)
    //Preserve value of reserved bit 9 and 8
    //Enable UART1, bit 0
    UART1_CTL_R |= 0b0011000000000001;        // enable UART1

}

void uart_sendChar(char data)
{
   while(UART1_FR_R & 0b00100000){};
   UART1_DR_R = data;

}

char uart_receive(void)
{
    char data = 0;
    while(UART1_FR_R & 0x00000010){};
      data = (char)(UART1_DR_R & 0xFF);
      return data;

}

void uart_sendStr(const char *data)
{
  while(*data != '\0'){
      uart_sendChar(*data);
      data++;
  }
   uart_sendChar('\r');
   uart_sendChar('\n');
}

// _PART3


void uart_interrupt_init()
{
    // Enable interrupts for receiving bytes through UART1
    //Set vits 5 and 4 to send an interrupt when the TXRIS and RXRIS bits in the UARTRIS register is set
    UART1_IM_R |= 0b00010000; //enable interrupt on receive - page 924

    // Find the NVIC enable register and bit responsible for UART1 in table 2-9
    // Note: NVIC register descriptions are found in chapter 3.4
    //Enable register 6 of the enable 0 registers of NVIC
    NVIC_EN0_R |= 0b01000000; //enable uart1 interrupts - page 104

    // Find the vector number of UART1 in table 2-9 ! UART1 is 22 from vector number page 104
    IntRegister(INT_UART1, uart_interrupt_handler); //give the microcontroller the address of our interrupt handler - page 104 22 is the vector number

}

void uart_interrupt_handler()
{

    if(UART1_MIS_R & 0x10){
    //received a byte
       uart_data = (char) (UART1_DR_R & 0xFF);
       uart_sendChar(uart_data);
       lcd_printf("%c", uart_data);
       UART1_ICR_R |= 0b00010000; //clear interrupt
     //  flag = 1;
 }
    if(UART1_MIS_R & 0x20){
        uart_data = (char) (UART1_DR_R & 0xFF);
              uart_sendChar(uart_data);
              lcd_printf("%c", uart_data);
              UART1_ICR_R |= 0b00100000; //clear interrupt
    }




}
