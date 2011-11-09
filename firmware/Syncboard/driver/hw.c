/*
 * main driver initialisation
 */
#include <avr/io.h>
#include <avr/interrupt.h>  // for sei()

#include "adc.h"

void hw_init() {
	// LEDs
	DDRD = (1 << PD6) | (1 << PD7) ;

	// Initialize the ADC
	adc_init();


	// initialize blink
	TCCR0B |= (1 << CS00) | (1 << CS02); // Prescaler: 1024
	TIMSK0 |= (1 << OCIE0A); // enable interrupt

	// Enable interrupts
	sei();
}

void setGreenLed(uint8_t led) {
	if(led) {
		PORTD |= (1 << PD7);
	} else {
		PORTD &= ~(1 << PD7);
	}
}


static uint8_t ledBlink = 0;

/**
 * Timer interrupt
 */
ISR(TIMER0_COMPA_vect) {
	ledBlink = !ledBlink;
	if(ledBlink) {
		PORTD |= (1 << PD6);
	} else {
		PORTD &= ~(1 << PD6);
	}
}

