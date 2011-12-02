#ifndef __LED_CONFIG_H
#define __LED_CONFIG_H

#include <stdint.h>

//3 Farben, 
#define PWM_STEPS  15
#define NUM_COLORS 3
#define LEDS_PER_BOARD   64
#define FRAME_SIZE PWM_STEPS * NUM_COLORS * LEDS_PER_BOARD / 8 //2880 bits = 360 bytes
//this number is important, so that 
#define NUM_FRAMES 128 // least common multiple: 46080
#define NUM_ROWS 1000

#define NUM_USB_FRAMES 720 //new value 45 //1024 bytes per usb frame

#define FRAME_BUFFER_LENGTH     NUM_FRAMES * FRAME_SIZE / 2 //because of word

extern uint16_t current_usb_frame;
extern uint16_t frame_buffer[];

#endif
