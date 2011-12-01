/**
 * Hardware abstraktion
 *
 * @author Andreas Butti
 */

#ifndef HW_H_
#define HW_H_

#include "../time/time.h"

void hw_init();
void setGreenLed(uint8_t led);
void getTime(struct Time * time);

#endif /* HW_H_ */
