/*
 * main driver initialisation
 */
#include <avr/io.h>
#include <avr/interrupt.h>  // for sei()
#include <util/atomic.h>

#include "hw.h"
#include "adc.h"

uint32_t time = 0;

void hw_init() {
	// LEDs
	DDRD = (1 << PD6) | (1 << PD7);

	// Initialize the ADC
	adc_init();

	// initialize timer
	TCCR1B = (1 << CS10); // Prescaler 1
	// TCCR1B = (1 << CS10) | (1 << CS12); // Prescaler 1024
	TIMSK1 |= (1 << OCIE1A); // enable timer

	// Enable interrupts
	sei();
}

void setGreenLed(uint8_t led) {
	if (led) {
		PORTD |= (1 << PD7);
	} else {
		PORTD &= ~(1 << PD7);
	}
}

void getTime(struct Time * t) {
	uint8_t sregTtmp;
	sregTtmp = SREG; // Sichern
	cli();
	// Interrupts deaktivieren

	t->parts = TCNT1;
	t->time = time;

	SREG = sregTtmp; // Wiederherstellen
}

static uint8_t ledBlink = 0;

/**
 * Timer interrupt
 */
ISR(TIMER1_COMPA_vect)
{
	ledBlink = !ledBlink;
	if (ledBlink) {
		PORTD |= (1 << PD6);
	} else {
		PORTD &= ~(1 << PD6);
	}

	time++;
}

