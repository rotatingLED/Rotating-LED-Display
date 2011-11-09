/*
 * @author Andreas Butti
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "driver/adc.h"
#include "driver/hw.h"

/**
 * Main Loop
 */
int main(void) {

	hw_init();

	while(1) {
		uint8_t v = adc_read(7);
		setGreenLed(v > 60);
	}

//
//	uint8_t umgebung = ADCRead(0);
//	uint16_t avg = umgebung;
//	uint8_t current;
//
//
//	while(1) {
//		current = ADCRead(0);
//		if(current - umgebung > 10) {
//			PORTB &= ~(1 << PB0);
//		} else {
//			PORTB |= (1 << PB0);
///*			avg = umgebung + current;
//			avg = avg >> 1; // Geteilt durch 2
//			umgebung = avg;*/
//		}
//
///*		if(PIND & (1 << PD5)) {
//			PORTD &= ~(1 << PD4);
//		} else {
//			PORTD = (1 << PD4);
//		}*/
//	}

	return 0;
}

