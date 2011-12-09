/**
 * Utility functions
 *
 * @author Andreas Butti
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdint.h>
#include "time/time.h"

void uartPutTime(struct Time * t);
void clearString16(char * buffer);
uint8_t myatoi(const char * str, uint16_t * i);
uint8_t readString16(char * buffer, const char * delim);

#endif /* __UTILS_H__ */
