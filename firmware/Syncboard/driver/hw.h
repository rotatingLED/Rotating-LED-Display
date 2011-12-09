/**
 * Hardware abstraktion
 *
 * @author Andreas Butti
 */

#ifndef __HW_H__
#define __HW_H__

#include "../time/time.h"

void hw_init();
void setYellowLed(uint8_t led);
void getTime(struct Time * time);
void syncInteruptOnTime(struct Time * time);

extern uint8_t ledMask;
extern uint8_t debugInterruptPrescaler;

#endif /* __HW_H__ */
