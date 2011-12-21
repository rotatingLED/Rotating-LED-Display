#include "adc.h"

/*
 * Init ADC
 */
void adc_init() {
	ADMUX = (0 << REFS1) | (1 << REFS0); // AVCC with external capacitor at AREF pin
	ADMUX |= (1 << ADLAR); // 255 Steps, 8bit
	ADMUX |= 7; // X => ADCX selected

	// set analog to digital converter
	// to be enabled, with a clock prescale of 1/128
	// so that the ADC clock runs at 93.7 kHz.
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

	// fire a conversion just to get the ADC warmed up
	adc_read();
}

void adc_select(uint8_t adc) {
	ADMUX &= ~7;
	ADMUX |= adc; // X => ADCX selected
}


uint8_t adc_read() {
	ADCSRA |= (1 << ADSC); // eine Wandlung "single conversion"
	while (ADCSRA & (1 << ADSC))
		; // auf Abschluss der Konvertierung warten
	return ADCH; // ADC auslesen und zurückgeben (8bit, ansonsten ADCW)
}

