/**
 * Time handling implementation
 *
 * @author Andreas Butti
 */

#include "time.h"

/**
 * c = a - b
 */
void timeSub(struct Time * a, struct Time * b, struct Time * c) {
	uint8_t timeDiff = 0;
	if (a->parts < b->parts) {
		timeDiff = 1;
	}

	c->parts = a->parts - b->parts;
	c->time = a->time - b->time - timeDiff;
}

/**
 * c = a + b
 */
void timeAdd(struct Time * a, struct Time * b, struct Time * c) {
	uint32_t parts = a->parts + b->parts;

	c->time = a->time + b->time;
	if(parts > 65535) {
		c->time++;
	}
	c->parts = parts;
}
