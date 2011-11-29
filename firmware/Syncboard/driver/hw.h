/*
 * hw.h
 *
 *  Created on: 09.11.2011
 *      Author: andreas
 */

#ifndef HW_H_
#define HW_H_

void hw_init();
void setGreenLed(uint8_t led);


struct Time {
	uint32_t time;
	uint16_t parts;
};

void getTime(struct Time * time);

/**
 * c = a - b
 */
void timeDiff(struct Time * a, struct Time * b, struct Time * c);

#endif /* HW_H_ */
