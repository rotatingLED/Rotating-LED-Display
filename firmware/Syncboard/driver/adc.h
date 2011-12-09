/**
 * ADC handler, currently only used for accu
 */

#ifndef __ADC_H__
#define __ADC_H__

#include <avr/io.h>

/*
 * Init ADC
 */
void adc_init();
uint8_t adc_read();
void adc_select(uint8_t adc);

#endif /* __ADC_H__ */
