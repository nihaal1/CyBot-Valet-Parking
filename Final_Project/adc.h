/*
 * adc.h
 *
 *  Created on: Jun 10, 2022
 *      Author: vsadvani
 */

#ifndef ADC_H_
#define ADC_H_




#include <stdint.h>
#include <stdbool.h>
#include <inc/tm4c123gh6pm.h>
#include "driverlib/interrupt.h"

/*
 * Initialize ADC
 */
void adc_init();

/*
 * Get raw value from IR sensor
 */
int adc_read();

#endif /* ADC_H_ */
