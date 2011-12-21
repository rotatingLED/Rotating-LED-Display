/*
 * main driver initialisation
 */
#include <avr/io.h>
#include <avr/interrupt.h>  // for sei()
#include <util/atomic.h>

#include "uart.h"

#include "hw.h"
#include "adc.h"

volatile uint32_t time = 0;

void hw_init() {
	// LEDs
	DDRD = (1 << PD6) | (1 << PD7);

	// Initialize the ADC
	adc_init();

	// initialize timer
	TCCR1B = (1 << CS10); // Prescaler 1
	// TCCR1B = (1 << CS10) | (1 << CS12); // Prescaler 1024
	TIMSK1 |= (1 << OCIE1A) | (1 << OCIE1B); // enable timer

	DDRC |= (1 << PC3); // INTERRUPT OUTPUT

	// Enable interrupts
	sei();
}

void setYellowLed(uint8_t led) {
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
 * Interrupt auf Leitung ausgeben
 */
void sendInterrupt() {
	PORTC |= (1 << PC3);
	asm volatile("NOP");
	asm volatile("NOP");
	asm volatile("NOP");
	PORTC &= ~(1 << PC3);
}


static uint8_t ledBlink = 0;

static uint8_t prescalerCount = 0;

/**
 * Timer interrupt (regelmässig)
 */
ISR(TIMER1_COMPA_vect)
{
	ledBlink = !ledBlink;
	if (ledBlink && (ledMask & 0x01)) {
		PORTD |= (1 << PD6);
	} else {
		PORTD &= ~(1 << PD6);
	}

	if(debugInterruptPrescaler != 0xff){
		if(prescalerCount == 0) {
			simulateEvent();
			prescalerCount = debugInterruptPrescaler;
		} else {
			prescalerCount--;
		}
	}

	time = time + 1;
}

struct Time syncTime;
uint8_t timePending = 0;

void syncInteruptOnTime(struct Time * time) {
	syncTime = *time;
	OCR1B = syncTime.parts;
	timePending = 1;
}

/**
 * Timer interrupt für LED Synchonisationsinterrupt
 */
ISR(TIMER1_COMPB_vect)
{
	if(timePending) {
		if(time == syncTime.time) {
			sendInterrupt();
			timePending = 0;
		} else if(time > syncTime.time) {
			sendInterrupt();
			//uart_puts("err SYNC_TIME_TO_SHORT\n");
			timePending = 0;
		}
	}
}

