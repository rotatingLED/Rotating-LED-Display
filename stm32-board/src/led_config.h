#ifndef __LED_CONFIG_H
#define __LED_CONFIG_H

#include <stdint.h>

// CAUTION: the comments with the numbers are not always right anymore!
//3 Farben, 
#define PWM_STEPS               2
#define NUM_COLORS              3
#define LEDS_PER_BOARD          64

// 3*128 ~ about 1 frame 
#define NUM_FRAMES              (9*128)//old 128 // least common multiple: 46080
#define NUM_ROWS                344

// 2880 bits = 360 bytes
// new:  384 =  48 bytes
#define FRAME_SIZE              (PWM_STEPS * NUM_COLORS * LEDS_PER_BOARD / 8)
#define IMAGE_SIZE              (NUM_ROWS * FRAME_SIZE)

#define FRAME_BUFFER_LENGTH     (NUM_FRAMES * FRAME_SIZE / 2) //because of word

#define NUM_USB_FRAMES          (FRAME_BUFFER_LENGTH * 2 / 64) //old 720 //new value 45 //1024 bytes per usb frame

#define NUM_FRAMES_IN_MULTI_FRAME          8

#define MULTI_FRAME_SIZE        (FRAME_SIZE * NUM_FRAMES_IN_MULTI_FRAME) //old  2880 -> 16 multi frames in buffer

#define NUM_MULTI_FRAMES        (FRAME_BUFFER_LENGTH *2 / MULTI_FRAME_SIZE) //144

#define MULTI_FRAMES_IN_IMAGE   (NUM_ROWS / NUM_FRAMES_IN_MULTI_FRAME)
#define NUM_USB_FRAMES_IN_MULTI_FRAME      (MULTI_FRAME_SIZE / 64)       // old 45

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
