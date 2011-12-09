#include "adc.h"

/*
 * Init ADC
 */
void adc_init() {
	ADMUX  = (0 << REFS1) | (1 << REFS0); // AVCC with external capacitor at AREF pin
	ADMUX |= (1 << ADLAR); // 255 Steps, 8bit
	ADMUX |= 0; // X => ADCX selected

	ADCSRA  = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler: 128
	ADCSRA |= (1 << ADEN); // enable ADC

	/* nach Aktivieren des ADC wird ein "Dummy-Readout" empfohlen, man liest
	  also einen Wert und verwirft diesen, um den ADC "warmlaufen zu lassen" */

	adc_read();
}

void adc_select(uint8_t adc) {
	ADMUX &= ~0x7; // Letze 3 bit löschen
	ADMUX |= adc & 0x7; // X => ADCX selected
}


uint8_t adc_read() {
	ADCSRA |= (1 << ADSC);            // eine Wandlung "single conversion"
	while(ADCSRA & (1 << ADSC));      // auf Abschluss der Konvertierung warten
	return ADCH;                      // ADC auslesen und zurückgeben (8bit, ansonsten ADCW)
}


