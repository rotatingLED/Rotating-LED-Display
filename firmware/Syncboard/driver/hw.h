/**
 * Hardware abstraktion
 *
 * @author Andreas Butti
 */

#ifndef __HW_H__
#define __HW_H__

#include "../time/time.h"

void hw_init();
void setGreenLed(uint8_t led);
void getTime(struct Time * time);

#endif /* __HW_H__ */
