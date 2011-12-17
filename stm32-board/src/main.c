/*******************************************************************************
 * File Name          : main.c
 * Author             : Martin Thomas, main-skeleton based on code from the
 *                      STMicroelectronics MCD Application Team
 * Version            : see VERSION_STRING below
 * Date               : see VERSION_STRING below
 * Description        : Main program body for the SD-Card tests
 ********************************************************************************
 * License: 3BSD
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32f10x.h"
#include "stm32f10x_exti.h"

#include "platform_config.h"

#include <errdisplay.h>

#include "usb/usb_lib.h"
#include "usb/usb_desc.h"
#include "usb/hw_config.h"
#include "usb/usb_pwr.h"

#include "src/led_config.h"

/* Private function prototypes -----------------------------------------------*/
void Periph_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void ExtInterrupt_Configuration();
void Timer_Configuration();

void stdio_tests(void);
void dcc_tests(void);

/* Public functions -- -------------------------------------------------------*/

/**
 * Sys clock handler
 */
void SysTick_Handler(void) {
  if (row_clock_counter % 1000 > 500){
    GPIOB->ODR = (1 << 10); //color: blue
  GPIOG->ODR = 0xFFFF;
  GPIOG->ODR = 0;
  }else{
    GPIOB->ODR = (1 << 11);
  GPIOG->ODR = 0xFFFF;
  GPIOG->ODR = 0;
  }
  row_clock_counter += 0x10000;
  row_clock_counter &= 0xFFFF0000;
  //row_clock_counter++;
  /*
  cnt++;
  if (cnt >= 1000) {
    cnt = 0;
    //errdisplay_aliveSign();
  }
  */
}

void USB_HP_CAN1_TX_IRQHandler(void) {
}

/**
 * Main entry point
 */
int main(void) {
  // System Clocks Configuration
  Periph_Configuration();

  // NVIC configuration
  NVIC_Configuration();

  // Configure the GPIO ports
  GPIO_Configuration();

  // Sync interrupt configuration
  ExtInterrupt_Configuration();
  __enable_irq();

  // Initialize Timer
  Timer_Configuration();

  // Setup SysTick Timer for 1 millisecond interrupts, also enables Systick and Systick-Interrupt
  // needed for SysTick_Handler
  if (SysTick_Config(SystemCoreClock / 1000)) {
    // Display error
    errdisplay_displayErr(ERROR_INIT_SYSCLOCK_FAILED);
  }

  // USB example
  Set_System();
  Set_USBClock();
  USB_Interrupts_Config();
  USB_Init();

  // Turn on/off LED(s)
  register int current_led = -1;
  register int current_row = 0;
  uint32_t time_temp = 0;

  int i = 0;
  uint32_t time_per_column = 500 << 16;
  uint32_t time_end = 0;
  uint16_t led_temp[2] = {0xFFFF, 0};
  /*
  while(1){
    asm volatile ("cpsid i");
    time_end = row_clock_counter + SysTick->VAL + time_per_column;
    time_temp = row_clock_counter + SysTick->VAL;
    time_end = row_clock_counter + time_per_column;
    GPIOB->ODR = (1 << 10); //color: blue
    GPIOD->ODR = led_temp[i];
    ++i;
    i = i % 2;

    asm volatile ("cpsie i");
    
    while(time_end > time_temp){ //TIM_GetCounter(TIM3)
      time_temp = row_clock_counter + SysTick->VAL;
    }
  }
  */
  while (1) {
    /*
     GPIOB->ODR = 0xffff;
     i++;
     GPIOD->ODR = 0xffff;
     GPIOE->ODR = 0xffff;
     GPIOF->ODR = 0xffff;
     GPIOG->ODR = 0xffff;

     sleep(1);
     GPIOD->ODR = 0x0000;
     GPIOE->ODR = 0x0000;
     GPIOF->ODR = 0x0000;
     GPIOG->ODR = 0x0000;
     */

    //Interrupts sperren
    asm volatile ("cpsid i");

    // wait for synchronisation -> just show red
    if (synchro_enable == 1 || (wait_for_start == 1 && start_frame_interrupt)){
      GPIOB->ODR = (1 << 11); //color: red
      GPIOD->ODR = 0xFFFF;
      GPIOE->ODR = 0xFFFF;
      GPIOF->ODR = 0xFFFF;
      GPIOG->ODR = 0xFFFF;
      continue;
    }

    // when the frame interrupt came, get the next frame
    if (start_frame_interrupt == 1){
      start_frame_interrupt = 0;

      uint32_t mod_multi_frame = current_multi_frame % MULTI_FRAMES_IN_IMAGE;
      current_multi_frame = current_multi_frame - mod_multi_frame 
                                      + MULTI_FRAMES_IN_IMAGE;
      if (endp3_locked == 1){
          SetEPRxValid(ENDP3);
      }
      if (current_multi_frame >= current_usb_multi_frame){
        // this is a buffer underrun, the interrupt already came
        // and the buffer is not ready, therefore enable synchro
        // enable_synchronisation();
        // continue;
      }else{
        // here we have the data, just reset the stuff right
        // -1 because current_led starts negative
        current_led = mod_multi_frame * MULTI_FRAME_SIZE - 1;
        current_row = 0;
      }
    }else{
      //two things to test in this part: buffer underruns and overruns
      
      time_temp = row_clock_counter + SysTick->VAL;
      // check if current_led needs to be set back,
      // because the clock is not ready, yet
      if (row_interrupt_time + current_row*time_per_pixel < time_temp){
        //check if the interrupt has not come, then it has nothing todo with time
        // set current_led to the last pixel
        current_led -= FRAME_SIZE;
      }else{
        // here we have the case, that the time needs to be left as it is
        // or set forward except the image is in the last row, then always set back
        if (current_row == NUM_ROWS-1){
          current_led -= FRAME_SIZE;
        }else{
          // add rows
          current_row++;
          // if there was no interrupt: set the actual current_led
          if (current_led >= (FRAME_BUFFER_LENGTH - 1)) {
            current_led = -1;
          }
          // check for multi frames
          if ((current_led % MULTI_FRAME_SIZE) == (MULTI_FRAME_SIZE - 1)){
            current_multi_frame++;
            // unlock usb endpoint 3 which is the data carrier
            if (endp3_locked == 1){
              endp3_locked = 0;
              // set the endpoint free!
              release_endp3 = 1;
            }
            // now check for whole images, so that it's never possible to jump to
            // the next image, if the interrupt for the next image came
          }
        }
      }

    }
    GPIOB->ODR = (1 << 10); //color: blue
    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = 0;
    GPIOE->ODR = 0;
    GPIOF->ODR = 0;
    GPIOG->ODR = 0;

    GPIOB->ODR = (1 << 11); //color: red

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOC->ODR = 0;
    GPIOD->ODR = 0;
    GPIOE->ODR = 0;
    GPIOF->ODR = 0;
    GPIOG->ODR = 0;

    GPIOB->ODR = (1 << 12); //color: green

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];

    GPIOD->ODR = frame_buffer[++current_led];
    GPIOE->ODR = frame_buffer[++current_led];
    GPIOF->ODR = frame_buffer[++current_led];
    GPIOG->ODR = frame_buffer[++current_led];

    GPIOD->ODR = 0;
    GPIOE->ODR = 0;
    GPIOF->ODR = 0;
    GPIOG->ODR = 0;

    // this needs to be at the end, because you could have a buffer overrun,
    // if you gave it free in the beginning
    if (release_endp3 == 1){
      release_endp3 = 0;
      SetEPRxValid(ENDP3);
    }
    asm volatile ("cpsie i");
    //uint16_t value = TIM_GetCounter(TIM3);
    /*
     i++;
     GPIOC->ODR = 0xffff;
     GPIOD->ODR = 0xffff;
     GPIOE->ODR = 0xffff;
     GPIOF->ODR = 0xffff;
     GPIOG->ODR = 0xffff;

     GPIOC->ODR = 0x0000;
     GPIOD->ODR = 0x0000;
     GPIOE->ODR = 0x0000;
     GPIOF->ODR = 0x0000;
     GPIOG->ODR = 0x0000;
     sleep(1);

     if (i%6000 == 0){
     GPIOB->ODR = ~(1 << 10);
     }else if (i%6000 == 2000){
     GPIOB->ODR = ~(1 << 11);
     }else if (i%6000 == 4000){
     GPIOB->ODR = ~(1 << 12);
     }
     */
    /*GPIOD->ODR = 0x0400;

     GPIOC->ODR = 0xffff;
     //sleep(2000);
     GPIOC->ODR = 0x0000;*/
    //sleep(2000);
    //GPIOC->ODR = 0xFBFF;
    //GPIOD->ODR = 0x0000;
    /*
     //mine:
     GPIOC->ODR = dummy[++i];
     dummy[0] = dummy2[0];
     dummy[1] = dummy2[1];
     dummy[2] = dummy2[2];
     //dummy[3] = dummy2[3];
     GPIOC->ODR = dummy[++i];
     GPIOC->ODR = dummy[++i];
     GPIOC->ODR = dummy[++i];
     GPIOC->ODR = 0xffff;
     GPIOC->ODR = 0x0000;
     i = 0;
*/
     //GPIOD->ODR = 0xFFFF;
     /*GPIOC->BRR = GPIO_Pin_All;
     GPIOC->BSRR = GPIO_Pin_All;
     GPIOC->BRR = GPIO_Pin_All;
     GPIOC->BSRR = GPIO_Pin_All;*/
  }
}

void Timer_Configuration() {
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  // TIM3->DIER = TIM_DIER_UIE; // Update interrupt enable
  TIM3->CR1 = TIM_CR1_CEN;
  // TIM3->ARR = 562;
  TIM3->ARR = 250;
  TIM3->PSC = 0; 
  //old
  // f=72MHz, ARR=5, PSC=0 => fpixclk = 12MHz
  //TIM3->ARR //Auto reload register

}

void ExtInterrupt_Configuration() {
  //EXTI structure to init EXT
  EXTI_InitTypeDef EXTI_InitStructure;
  //NVIC structure to set up NVIC controller
  NVIC_InitTypeDef NVIC_InitStructure;
  //Connect EXTI Line to Button Pin
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);
  //Configure Button EXTI line
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  //select interrupt mode
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  //generate interrupt on rising edge
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  //enable EXTI line
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  //send values to registers
  EXTI_Init(&EXTI_InitStructure);
  //configure NVIC
  //select NVIC channel to configure
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  //set priority to lowest
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
  //set subpriority to lowest
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
  //enable IRQ channel
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //update NVIC registers
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
 * Function Name  : PeriphConfiguration
 * Description    : Configures the different system clocks.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void Periph_Configuration(void) {
  /* Enable USART1, GPIOA, GPIOD and AFIO clocks */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
      | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG | RCC_APB2Periph_AFIO, ENABLE);
  /* Enable USART2 clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  /* Enable GPIO_LED clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_LED, ENABLE);

  /* DMA1 clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

/*******************************************************************************
 * Function Name  : GPIO_Configuration
 * Description    : Configures the different GPIO ports.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
void GPIO_Configuration(void) {
  GPIO_InitTypeDef GPIO_InitStructure;

#if defined(USE_STM3210B_EVAL) || defined(USE_EK_STM32F)
  /* Enable the USART2 Pins Software Remapping */
  GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
#endif

  /* Configure USART1 TX (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART2 TX as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_TxPin;
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  /* Configure USART1 RX (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART2 RX as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_RxPin;
  GPIO_Init(GPIOx, &GPIO_InitStructure);

  /* Configure GPIO for LEDs as Output push-pull */
  // GPIO B
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // GPIO B13 Interrupt pin
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  // GPIO C
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  // GPIO D
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  // GPIO E
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  // GPIO F
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

  // GPIO G
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
#if defined(USE_MINI_STM32)
  /* touch-controller's CS (PA4), SD-Card's CS (PB6) and DataFlash CS (PB7) high = unselect */
  /* PB6 and PB7 both have an external 4,7kOhm pull-up */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
#if 1
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA, GPIO_Pin_4);
#endif
#endif /* defined(USE_MINI_STM32) */
}

/*******************************************************************************
 * Function Name  : NVIC_Configuration
 * Description    : Configures Vector Table base location.
 * Input          : None
 * Output         : None
 * Return         : None
 *******************************************************************************/
#ifdef VECT_TAB_RAM
/* vector-offset (TBLOFF) from bottom of SRAM. defined in linker script */
extern uint32_t _isr_vectorsram_offs;
void NVIC_Configuration(void)
{
  /* Set the Vector Table base location at 0x20000000+_isr_vectorsram_offs */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, (uint32_t)&_isr_vectorsram_offs);
}
#else
extern uint32_t _isr_vectorsflash_offs;
void NVIC_Configuration(void) {
  /* Set the Vector Table base location at 0x08000000+_isr_vectorsflash_offs */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, (uint32_t) & _isr_vectorsflash_offs);
}
#endif /* VECT_TAB_RAM */

/*******************************************************************************
 * Function Name  : assert_failed
 * Description    : called on failed assertion if compiled with USE_FULL_ASSERT
 * Input          : pointers to char-arrays/strings: filename, function-name,
 *                  line in file
 * Output         : via xprintf
 * Return         : None
 *******************************************************************************/
#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *   where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval : None
 */
void assert_failed(const uint8_t* file, const uint8_t* function, uint32_t line) {
  /* User can add his own implementation to report the file name and line number,
   ex: printf("Wrong parameters value: file %s on line %d\n", file, line) */

  file = file; // avoid warnings
  function = function; // avoid warnings
  line = line; // avoid warnings

  errdisplay_displayErr(ERROR_ASSERTION_FAILED);
}
#endif
