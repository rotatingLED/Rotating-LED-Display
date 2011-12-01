#include <avr/io.h>
#include "utils.h"
#include "driver/uart.h"
#include <string.h>
#include "time/time.h"
#include <util/delay.h> // _delay_ms


void uartPutTime(struct Time * t) {
	uint8_t * ptr = (uint8_t *) t;
	uint8_t i;

	for (i = 0; i < sizeof(struct Time); i++) {
		uart_puth(*ptr);
		ptr++;
	}
}

void clearString16(char * buffer) {
	uint8_t i;
	for (i = 0; i < 16; i++) {
		buffer[i] = 0;
	}
}

uint8_t myatoi(const char * str, uint16_t * i) {
	*i = 0;
	uint8_t v;

	while (*str) {
		v = ((*str) - '0');
		if (v >= 10) {
			return 1;
		}
		(*i) *= 10;
		(*i) += v;
		str++;
	}
	return 0;
}

uint8_t readString16(char * buffer, const char * delim) {
	unsigned int chr;
	char c;
	uint8_t i = 0;

	while (1) {
		chr = uart_getc();

		if (chr & UART_FRAME_ERROR) {
			/* Framing Error by UART       */
			uart_puts("uer UART_FRAME_ERROR\n");
			return 1;
		}
		if (chr & UART_OVERRUN_ERROR) {
			/* Overrun condition by UART   */
			uart_puts("uer UART_OVERRUN_ERROR\n");
			return 1;
		}
		if (chr & UART_FRAME_ERROR) {
			/* receive ringbuffer overflow */
			uart_puts("uer UART_BUFFER_OVERFLOW\n");
			return 1;
		}
		if (chr & UART_NO_DATA) {
			/* no receive data available   */
			_delay_ms(1);
			continue;
		}

		c = chr;

		if (i >= 15) {
			return 2; // overflow
		}

		if (strrchr(delim, c) != NULL) {
			return 0; // end found
		}

		buffer[i++] = c;
	}
}

