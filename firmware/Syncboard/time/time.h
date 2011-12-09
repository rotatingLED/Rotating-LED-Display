/**
 * Time handling implementation
 *
 * @author Andreas Butti
 */

#include <stdint.h>

#ifndef __TIME_H__
#define  __TIME_H__

struct Time {
	uint32_t time;
	uint16_t parts;
};

/**
 * c = a - b
 */
void timeSub(struct Time * a, struct Time * b, struct Time * c);

/**
 * c = a + b
 */
void timeAdd(struct Time * a, struct Time * b, struct Time * c);

/**
 * c = a / b
 */
void timeDiff(struct Time * a, uint8_t b, struct Time * c);

#endif //  __TIME_H__
