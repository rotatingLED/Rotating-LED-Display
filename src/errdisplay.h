/*
 * Rotating LED Display
 * ZHAW PA Project
 *
 * Author: Andreas Butti (buttiand), David Halter (haltedav)
 *
 * Brief:
 * Displays errors on the LED on PC13
 *
 * Short light: 0
 * Long light: 1
 *
 * Most significant bit first, seperated by a break
 */

#ifndef __ERRDISPLAY_H__
#define __ERRDISPLAY_H__

typedef enum {
	ERROR_INIT_SYSCLOCK_FAILED = 1,
	ERROR_EXIT = 2,
	ERROR_HEAP_STACK_COLLISION = 3,
	ERROR_ASSERTION_FAILED = 4

} ErrorCodes;

// error led connection
#define ERROR_STATE_LED_PORT GPIOC
#define ERROR_STATE_LED_PIN GPIO_Pin_13

/**
 * Display an error code on Display, never returns
 */
__attribute__((noreturn)) void errdisplay_displayErr(ErrorCodes err);

/**
 * Displays the system is still alive
 */
void errdisplay_aliveSign();

#endif /* __ERRDISPLAY_H__ */
