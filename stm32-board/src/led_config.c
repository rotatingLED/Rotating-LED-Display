
#include "led_config.h"

uint16_t frame_buffer[FRAME_BUFFER_LENGTH];

uint16_t current_usb_frame = 0;

uint32_t timer_speed  = 1;
uint8_t synchro_enable = 1;
