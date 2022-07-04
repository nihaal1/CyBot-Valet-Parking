/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */
#ifndef PING_H_
#define PING_H_

#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"


/**
 * Initialize ping sensor. Uses PB3 and Timer 3B
 */
void ping_initialize(void);

/**
 * @brief Trigger the ping sensor
 */
void ping_activate(void);

/**
 * @brief Timer3B ping ISR
 */
void timer_handler(void);

//int pulse_width(void);

/**
 * @brief Calculate the distance in cm
 *
 * @return Distance in cm
 */
float ping_read ();
//int getOverflow(void);
#endif /* PING_H_ */
