/**
 * Time handling implementation
 *
 * @author Andreas Butti
 */

#include <stdint.h>

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

