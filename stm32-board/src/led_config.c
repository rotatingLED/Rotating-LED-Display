
#include "led_config.h"
#include "usb/usb_lib.h"

uint16_t frame_buffer[FRAME_BUFFER_LENGTH];

volatile uint16_t current_usb_frame = 0;

volatile uint32_t time_per_pixel = 500 << 16;
volatile uint8_t synchro_enable = 1;
volatile uint8_t wait_for_start = 1;

volatile uint32_t current_multi_frame = 0;
volatile uint32_t current_usb_multi_frame = 0;

volatile uint8_t endp3_locked = 0;
volatile uint8_t release_endp3 = 0;
volatile uint8_t start_frame_interrupt = 1;

volatile uint32_t row_clock_counter = 0;
volatile uint32_t row_interrupt_time = 0;

void enable_synchronisation(){
  // don't lock the interrupts, because it might be called
  // out of a locked interrupt state
  
  //asm volatile ("cpsid i");
  if (endp3_locked == 1){
      SetEPRxValid(ENDP3);
  }
  synchro_enable = 1;
  //asm volatile ("cpsie i");
}
