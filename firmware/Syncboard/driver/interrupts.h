/*
 * Interrupt definitions for Mega168
 */

#ifndef __INTERRUPTS_H__
#define __INTERRUPTS_H__

/**
 * Timer
 */
#define INIT_SENDER_TIMER() \
	TCCR1B = (1 << CS10) /* Timer 8 MHz*/

//	TCCR1B = (1 << CS11) /* Timer 1 MHz*/
//	TCCR1B = (1 << CS10) /* Timer 8 MHz*/

//	TCCR1B = (1 << CS12) | (1 << CS10); /* Timer 8 kHz !!DEBUGGING!!*/

#define TIMER_SENDER_ENABLE() \
	TIMSK1 |= (1 << OCIE1A)

#define TIMER_SENDER_DISABLE() \
	TIMSK1 &= ~(1 << OCIE1A)

#define TIMER_SENDER_NEXT(v)\
	OCR1A = v; \
	TCNT1 = 0

#define TIMER_SENDER_ISR TIMER1_COMPA_vect

/**
 * Laser
 */
#define IR_LED_ON() \
	PORTB |= (1 << PB0);

#define IR_LED_OFF() \
	PORTB &= ~(1 << PB0);


#endif /* __INTERRUPTS_H__ */
