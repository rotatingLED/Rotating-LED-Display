/**
  ******************************************************************************
  * @file platform_config.h
  * @author  MCD Application Team, mod. Martin Thomas for Manley EK-STM32F
  * @version  V3.0.0.1
  * @date  04/17/2009
  * @brief  Evaluation board specific configuration file.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#define GPIOx                               GPIOD
#define RCC_APB2Periph_GPIOx                RCC_APB2Periph_GPIOD
#define GPIO_TxPin                          GPIO_Pin_5
#define GPIO_RxPin                          GPIO_Pin_6
#define GPIO_LED                            GPIOC
#define RCC_APB2Periph_GPIO_LED             RCC_APB2Periph_GPIOC
#define GPIO_Pin_LED1                       GPIO_Pin_13


// USB Config
#define USB_DISCONNECT                      GPIOD
#define USB_DISCONNECT_PIN                  GPIO_Pin_9
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD
#define EVAL_COM1_IRQHandler                USART1_IRQHandler



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
