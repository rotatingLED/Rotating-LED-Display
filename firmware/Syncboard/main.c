/*
 * @author Andreas Butti
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "driver/adc.h"
#include "driver/hw.h"
#include "driver/uart.h"
#include <util/delay.h> // _delay_ms
#include "time/time.h"
#include "utils.h"
#include <string.h>
//#include <avr/wdt.h> // Watchdog
/**
 * UART Baudrate
 */
#define UART_BAUD_RATE 57600

/**
 * Configuration variables
 */
uint8_t adcThreshold = 255;
uint16_t interruptDelay = 100;
uint8_t displayAdcDebug = 0;

uint8_t ledMask = 0x03;

uint8_t debugInterruptPrescaler = 0xff;

uint8_t inConfiguration = 0;

/**
 * Times
 */

// Time when turned off
struct Time timeOff;

// Time when turned on
struct Time timeOn;

// The difference between on and off
struct Time diff;

// Center time betweeen on and off
struct Time centerOnTime;

// Last center time betweeen on and off
struct Time lastCenterOnTime;

/**
 *  Prototypes
 */
void configuration();
void handleLightDown();
void printAdc(uint8_t v);

/**
 * Main Loop
 */
int main(void) {
	uint8_t lastState = 0;
	uint8_t state = 0;
	uint8_t lastAdc = 0;
	uint8_t c;

	hw_init();
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE, F_CPU));

	uart_puts("\r\n\r\nstartup!\r\n");

	getTime(&centerOnTime);

	//adc_select(6);

	while (1) {
		//wdt_reset();
		uint8_t v = adc_read();

		if (debugInterruptPrescaler == 0xff) {
			if (displayAdcDebug) {
				if (lastAdc != v) {
					printAdc(v);
					lastAdc = v;
				}
			}

			state = v > adcThreshold;

			if (state != lastState) {
				lastState = state;

				if (state == 0) {
					getTime(&timeOff);

					handleLightDown();
				} else {
					getTime(&timeOn);
				}

				setYellowLed(state && (ledMask & 0x02));
			}
		}

		c = uart_getc();
		if (c == 'c') {
			inConfiguration = 1;

			configuration();

			inConfiguration = 0;
		} else if (c == 'a') {
			printAdc(v);
		}
	}

	return 0;
}

void printAdc(uint8_t v) {
	uart_puts("adc  ->");
	uart_puti(v);
	//	uart_putc(' ');
	//	uart_putc('0' + (PINC & (1 << 5) ? 1 : 0));
	uart_puts("\n");
}

void simulateEvent() {
	if (inConfiguration == 0) {
		getTime(&timeOff);
		handleLightDown();
		//		_delay_ms(2);

		timeOn = timeOff;

		//		getTime(&timeOn);
//		setYellowLed(ledMask & 0x02);
//		sendInterrupt();
//		setYellowLed(0);
	}
}

void handleLightDown() {
	struct Time tmp;
	struct Time rotationTimer;
	struct Time syncTime;

	timeSub(&timeOff, &timeOn, &diff);
	timeDiff(&diff, 2, &tmp);
	timeAdd(&tmp, &timeOff, &centerOnTime);
	timeSub(&centerOnTime, &lastCenterOnTime, &rotationTimer);

	tmp.parts = interruptDelay;
	tmp.time = 0;
	timeAdd(&centerOnTime, &tmp, &syncTime);

	syncInteruptOnTime(&syncTime);

	//	uart_puts("on=");
	//	uartPutTime(&timeOn);
	//	uart_puts("\n");
	//
	//	uart_puts("off=");
	//	uartPutTime(&timeOff);
	//	uart_puts("\n");
	//
	//	uart_puts("diff=");
	//	uartPutTime(&diff);
	//	uart_puts("\n");
	//
	//	uart_puts("cent=");
	//	uartPutTime(&centerOnTime);
	//	uart_puts("\n");

	uart_puts("rota=");
	uartPutTime(&rotationTimer);
	uart_puts("\n");

	//	uart_puts("sync=");
	//	uartPutTime(&syncTime);
	//	uart_puts("\n");
	//
	//	uart_puts("\n");

	lastCenterOnTime = centerOnTime;
}

uint8_t parseCmd(const char * command) {
	char name[16] = { 0 };
	char value[16] = { 0 };
	uint16_t iValue;

	if (strcmp(command, "list") == 0) {
		uart_puts("$ list\n");

		uart_puts("$ adc: adcThreshold = ");
		uart_puti(adcThreshold);
		uart_puts("\n");

		uart_puts("$ adcd: displayAdcDebug = ");
		uart_puti(displayAdcDebug);
		uart_puts("\n");

		uart_puts("$ int: interruptDelay = ");
		uart_puti(interruptDelay);
		uart_puts("\n");

		uart_puts("$ led: ledMask = ");
		uart_puti(ledMask);
		uart_puts("\n");

		uart_puts("$ dip: debugInterruptPrescaler = ");
		uart_puti(debugInterruptPrescaler);
		uart_puts("\n");

		uart_puts("$ boot0 = ");
		uart_puti((DDRC & (1 << 2)) == 0 ? 0 : 1);
		uart_puts("\n");

		uart_puts("$ reset = ");
		uart_puti((DDRB & (1 << 5)) == 0 ? 0 : 1);
		uart_puts("\n");

		uart_putc('>');
	} else if (strcmp(command, "set") == 0) {
		if (readString16(name, "=") == 0 && readString16(value, "\r\n ") == 0
				&& myatoi(value, &iValue) == 0) {
			uart_puts("$ set\n$ ");
			uart_puts(name);
			uart_putc('=');
			uart_puti(iValue);

			if (strcmp("adc", name) == 0) {
				adcThreshold = iValue;
			} else if (strcmp("adcd", name) == 0) {
				displayAdcDebug = iValue;
			} else if (strcmp("int", name) == 0) {
				interruptDelay = iValue;
			} else if (strcmp("led", name) == 0) {
				ledMask = iValue;
			} else if (strcmp("dip", name) == 0) {
				debugInterruptPrescaler = iValue;
			} else if (strcmp("boot0", name) == 0) {
				if (iValue) {
					// output schalten
					DDRC |= (1 << 2);
					PORTC |= (1 << 2);
				} else {
					// hochomig
					DDRC &= ~(1 << 2);
					PORTC &= ~(1 << 2);
				}
			} else if (strcmp("reset", name) == 0) {
				if (iValue) {
					// output schalten
					DDRB |= (1 << 5);
					PORTB &= ~(1 << 5);
				} else {
					// hochomig
					DDRB &= ~(1 << 5);
				}
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
//	} else if (strcmp(command, "reset") == 0) {
//		uart_puts("$ reset\n");
//
//		wdt_enable(WDTO_500MS);
//
//		while (1) {
//			_delay_ms(100);
//			setYellowLed(0);
//			_delay_ms(100);
//			setYellowLed(1);
//		}
//
//		return 0;
	} else {
		uart_puts("err UNKNOWN_COMMAND \"");

		uart_puts(command);

		uart_putc('"');
		uart_puts("\n");
		uart_putc('>');
	}

	return 1;
}

void configuration() {
	char command[16] = { 0 };

	uart_putc('>');

	while (1) {
		if (readString16(command, "\r\n ")) {
			return;
		}

		if (parseCmd(command) == 0) {
			break;
		}
		clearString16(command);
	}
}

