#include "errdisplay.h"
#include <types.h>
#include <stm32f10x_gpio.h>

static bool alive = true;

/**
 * Sleeping for a little time
 */
void sleep(uint32_t t) {
	for (; t > 0; t--) {
		for (uint16_t i = 0; i < 3000; i++) {
			// prevent optimizer from removing this loop
			asm volatile("NOP");
		}
	}
}

__attribute__((noreturn)) void errdisplay_displayErr(ErrorCodes err) {
	alive = false;

	while (true) {
		for (int8_t i = 3; i > -1; i--) {

			// Turn on LED
			GPIO_ResetBits(ERROR_STATE_LED_PORT, ERROR_STATE_LED_PIN);
			if (((1 << i) & err) == 0) {
				// short pulse
				sleep(500);
			} else {
				// long pulse
				sleep(1500);
			}

			GPIO_SetBits(ERROR_STATE_LED_PORT, ERROR_STATE_LED_PIN);
			sleep(1000);
		}

		// long break
		sleep(2000);
	}
}

void errdisplay_aliveSign() {
	static bool flip = true;

	if (alive) {
		if (flip) {
			// Turn on LED
			GPIO_ResetBits(ERROR_STATE_LED_PORT, ERROR_STATE_LED_PIN);
		} else {
			// Turn off LED
			GPIO_SetBits(ERROR_STATE_LED_PORT, ERROR_STATE_LED_PIN);
		}
		flip = !flip;
	}
}

