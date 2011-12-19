#ifndef __LED_CONFIG_H
#define __LED_CONFIG_H

#include <stdint.h>

//3 Farben, 
#define PWM_STEPS               15
#define NUM_COLORS              3
#define LEDS_PER_BOARD          64
#define NUM_FRAMES              128 // least common multiple: 46080
#define NUM_ROWS                1000

//2880 bits = 360 bytes
#define FRAME_SIZE              (PWM_STEPS * NUM_COLORS * LEDS_PER_BOARD / 8)
#define IMAGE_SIZE              (NUM_ROWS * FRAME_SIZE)

#define NUM_USB_FRAMES          720 //new value 45 //1024 bytes per usb frame

#define FRAME_BUFFER_LENGTH     (NUM_FRAMES * FRAME_SIZE / 2) //because of word

#define NUM_FRAMES_IN_MULTI_FRAME          8

#define MULTI_FRAME_SIZE        (FRAME_SIZE * NUM_FRAMES_IN_MULTI_FRAME) // 2880 -> 16 multi frames in buffer

#define MULTI_FRAMES_IN_IMAGE   (NUM_ROWS / NUM_FRAMES_IN_MULTI_FRAME)
#define NUM_USB_FRAMES_IN_MULTI_FRAME      (MULTI_FRAME_SIZE / 64)       // 45

void enable_synchronisation();

extern volatile uint16_t current_usb_frame;
extern uint16_t frame_buffer[];

extern volatile uint32_t time_per_pixel;
extern volatile uint32_t start_intterupt;
extern volatile uint8_t synchro_enable;
extern volatile uint8_t wait_for_start;

extern volatile uint32_t current_multi_frame;
extern volatile uint32_t current_usb_multi_frame;

extern volatile uint8_t endp3_locked;
extern volatile uint8_t release_endp3;
extern volatile uint8_t start_frame_interrupt;

extern volatile uint32_t row_clock_counter;
extern volatile uint32_t row_interrupt_time;
#endif
