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

	// initialize blink
	TCCR0B |= (1 << CS00) | (1 << CS02); // Prescaler: 1024: 12'000'000 / 1024 = 0.000085333s / Takt
	TIMSK0 |= (1 << OCIE0A); // enable interrupt

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

/**
 * c = a - b // c ist minimal 0
 */
void timeDiff(struct Time * a, struct Time * b, struct Time * c) {
	c->parts = 0;
	c->time = 0;

	if (a->time < b->time) {
		return;
	}

	uint8_t timeDiff = 0;
	if (a->parts < b->parts) {
		timeDiff = 1;
	}

	c->parts = a->parts - b->parts;
	c->time = a->time - b->time;
}

static uint8_t ledBlink = 0;

/**
 * Timer interrupt
 */
ISR(TIMER0_COMPA_vect)
{
	ledBlink = !ledBlink;
	if (ledBlink) {
		PORTD |= (1 << PD6);
	} else {
		PORTD &= ~(1 << PD6);
	}

//TODO: 	time++;
}

