/*
 * @author Andreas Butti
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "driver/adc.h"
#include "driver/hw.h"
#include "driver/uart.h"
#include <util/delay.h> // _delay_ms
#include <string.h>

#define UART_BAUD_RATE 57600

uint8_t adcThreshold = 60;

void configuration();

void uartPutTime(struct Time * t);

/**
 * Main Loop
 */
int main(void) {
	uint8_t lastState = 0;
	uint8_t state = 0;

	struct Time timeOff;
	struct Time timeOn;
	struct Time diff;

	hw_init();
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

	while (1) {
		uint8_t v = adc_read(7);
		state = v > adcThreshold;

		if (state != lastState) {
			lastState = state;

			if (state == 0) {
				getTime(&timeOff);

				timeDiff(&timeOn, &timeOff, &diff);

				uart_puts("on   =>");
				uartPutTime(&timeOn);
				uart_putc('\n');
				uart_puts("off  =>");
				uartPutTime(&timeOff);
				uart_putc('\n');
				uart_puts("diff =>");
				uartPutTime(&diff);
				uart_putc('\n');
			} else {
				getTime(&timeOn);
			}

			// TODO: handle change
		}

		if (uart_getc() == 'c') {
			configuration();
		}
	}

	return 0;
}

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

uint8_t parseCmd(const char * command) {
	char name[16] = { 0 };
	char value[16] = { 0 };
	uint16_t iValue;

	if (strcmp(command, "list") == 0) {
		uart_puts("$ list\n");
		uart_puts("$ adc: adcThreshold = ");
		uart_puti(adcThreshold);
		uart_putc('\n');
		uart_putc('>');
	} else if (strcmp(command, "set") == 0) {
		if (readString16(name, "=") == 0 && readString16(value, "\n\r ") == 0 && myatoi(value, &iValue) == 0) {
			uart_puts("$ set\n$ ");
			uart_puts(name);
			uart_putc('=');
			uart_puti(iValue);

			if (strcmp("adc", name) == 0) {
				adcThreshold = iValue;
			} else {
				uart_puts("\nerr UNKNOWN_VAR");
			}

			uart_puts("\n>");
		} else {
			uart_puts("err SET_VALUE\n>");
		}
	} else if (strcmp(command, "exit") == 0) {
		uart_puts("$ exit\n");
		return 0;
	} else {
		uart_puts("err UNKNOWN_COMMAND \"");

		uart_puts(command);

		uart_putc('"');
		uart_putc('\n');
		uart_putc('>');
	}

	return 1;
}

void configuration() {
	char command[16] = { 0 };

	uart_putc('>');

	while (1) {
		if (readString16(command, "\n\r ")) {
			return;
		}

		if (parseCmd(command) == 0) {
			break;
		}
		clearString16(command);
	}
}

