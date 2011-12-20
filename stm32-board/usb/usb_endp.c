/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_endp.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Endpoint routines
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"
#include "hw_config.h"
#include "usb_istr.h"
#include "usb_pwr.h"
#include "src/led_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Interval between sending IN packets in frame number (1 frame = 1ms) */
#define VCOMPORT_IN_FRAME_INTERVAL             5


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern  uint8_t USART_Rx_Buffer[];
extern uint32_t USART_Rx_ptr_out;
extern uint32_t USART_Rx_length;
extern uint8_t  USB_Tx_State;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback (void)
{
  uint16_t USB_Tx_ptr;
  uint16_t USB_Tx_length;
  
  if (USB_Tx_State == 1)
  {
    if (USART_Rx_length == 0) 
    {
      USB_Tx_State = 0;
    }
    else 
    {
      if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE){
        USB_Tx_ptr = USART_Rx_ptr_out;
        USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;
        
        USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;
        USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;    
      }
      else 
      {
        USB_Tx_ptr = USART_Rx_ptr_out;
        USB_Tx_length = USART_Rx_length;
        
        USART_Rx_ptr_out += USART_Rx_length;
        USART_Rx_length = 0;
      }
      
#ifdef USE_STM3210C_EVAL
      USB_SIL_Write(EP1_IN, &USART_Rx_Buffer[USB_Tx_ptr], USB_Tx_length);  
#else
      UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
      SetEPTxCount(ENDP1, USB_Tx_length);
      SetEPTxValid(ENDP1); 
#endif  
    }
  }
}

/*******************************************************************************
* Function Name  : EP3_OUT_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_OUT_Callback(void)
{
  uint16_t data_len;
  
  // Get the received data buffer and update the counter
  data_len = USB_SIL_Read(EP3_OUT, (uint8_t*)& frame_buffer[32*current_usb_frame]);
  
  // USB data will be immediately processed, this allow next USB traffic being 
  // NAKed till the end of the USART Xfer
  //if (frame_buffer[0] == 1){
  //if (data_len == 720){
  /*
  GPIOD->ODR = 0xffff;
  GPIOB->ODR = (1 << 11);
  sleep(10);
  GPIOD->ODR = 0x0000;
  for(int i=0;i<360;i++){
    if (frame_buffer[i] > 0){
      GPIOE->ODR = 0xffff;
      GPIOB->ODR = (1 << 10); //color: blue
      sleep(10);
      GPIOE->ODR = 0x0000;
    }
  }
  if (data_len == 1){
    GPIOF->ODR = 1;//0xffff;
    GPIOB->ODR = (1 << 12);
    sleep(10);
    GPIOF->ODR = 0x0000;
  }
  //}
  */
  
  //not used
  //USB_To_USART_Send_Data(frame_buffer, USB_Rx_Cnt);
  
/*  
  if (GetENDPOINT(ENDP3) & EP_DTOG_TX) {
    //read from ENDP1_BUF0Addr buffer
    data_len = GetEPDblBuf0Count(ENDP3);
    PMAToUserBufferCopy((uint8_t*)& frame_buffer[256*current_usb_frame], ENDP3_BUF0Addr, data_len);
    
//    if (data_len > 0){
//      //for(int i = 512*current_usb_frame;i<512*(current_usb_frame+1);i++){
//        //frame_buffer[i] = 1;
//      //}
//      if (frame_buffer[255] == 255){
//        GPIOC->ODR = 0xffff;
//        sleep(10);
//        GPIOC->ODR = 0x0000;
//      }
//    }

    //frame_buffer[256*current_usb_frame+128] = 0;
    if (frame_buffer[256*current_usb_frame+129] != 0){
      frame_buffer[256*current_usb_frame+129] = 0;
    }
    //frame_buffer[256*current_usb_frame+130] = 0;
//      for(int i = 256*current_usb_frame+129;i<(256*(current_usb_frame+1));i++){
//        frame_buffer[i] = 0;
//      }
  } else {
    //read from ENDP1_BUF1Addr buffer
    data_len = GetEPDblBuf1Count(ENDP3);
    PMAToUserBufferCopy((uint8_t*)& frame_buffer[256*current_usb_frame], ENDP3_BUF1Addr, data_len);
  }
*/

  //FreeUserBuffer(ENDP3, EP_DBUF_OUT); // double buffering

  // SerialPrintf("usb\r\n");
  if (synchro_enable == 1){
    //GPIOB->ODR = (1 << 12); GPIOD->ODR = 0xFFFF; sleep(500);
    uint8_t* p = & frame_buffer[32*current_usb_frame];
    int i = 0;
    if (p[0] == 255){
      for(i=1;i<64;i++){
        if (p[i] != 0){
          break;
        }
        if (i == 63){
          // now we have a synchro frame
          synchro_enable = 0;
          start_frame_interrupt = 0;
          wait_for_start = 1;

          // reset all the stuff
          current_usb_frame = 0;
          current_multi_frame = 0;
          current_usb_multi_frame = 0;
        }
      }
    }
    //if (wait_for_start == 1 && synchro_enable == 0){
    //  GPIOB->ODR = (1 << 10); GPIOD->ODR = 0xFFFF; sleep(500);
    //}
    //GPIOB->ODR = (1 << 12); GPIOD->ODR = 0xFFFF; sleep(500);
    SetEPRxValid(ENDP3);
  }else{
    if ((current_usb_frame % NUM_USB_FRAMES_IN_MULTI_FRAME) 
          == (NUM_USB_FRAMES_IN_MULTI_FRAME - 1)){
      current_usb_multi_frame++;
      // check if we have enough space for a new packet (buffer full)
      // underruns may not to be checked here, thats the job of the display loop
      if (current_usb_multi_frame >= (current_multi_frame + NUM_MULTI_FRAMES)){
        //SetEPRxValid(ENDP3); //must not be here
        // buffer is full -> wait
        endp3_locked = 1;
      }else{
        // Enable the receive of data on EP3
        SetEPRxValid(ENDP3);
      }
    }else{
      // Enable the receive of data on EP3
      SetEPRxValid(ENDP3);
    }

    // increase the usb frames and reset it if it reaches the 
    // maximal height of the buffer
    ++current_usb_frame;
    if (current_usb_frame >= NUM_USB_FRAMES){
      current_usb_frame = 0;
    }
  }
  // SerialPrintf("usb_end\r\n");
}

void EP1_OUT_Callback(void)
{
  uint16_t data_len;
  uint8_t buf_ctrl[8];
  
  // Get the received data buffer and update the counter
  data_len = USB_SIL_Read(EP1_OUT, buf_ctrl);

  // process:
  //   1: synchronisation enable - wait for synchronisation frame
  //   2: change timer speed
  //GPIOB->ODR = (1 << 12); GPIOD->ODR = 0xFFFF; sleep(500);
  if (buf_ctrl[0] == 1){
    synchro_enable = true;
    SerialPrintf("set synchro_enable=%u\r\n", synchro_enable);
  }else if (buf_ctrl[0] == 2){
    time_per_pixel = *((uint32_t*) & buf_ctrl[1]);
    SerialPrintf("set time_per_pixel=%u\r\n", time_per_pixel);
  }
#ifndef STM32F10X_CL
  // Enable the receive of data on EP4
  SetEPRxValid(ENDP1);
#endif
}

/*******************************************************************************
* Function Name  : SOF_Callback / INTR_SOFINTR_Callback
* Description    :
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
#ifdef STM32F10X_CL
void INTR_SOFINTR_Callback(void)
#else
void SOF_Callback(void)
#endif /* STM32F10X_CL */
{
  static uint32_t FrameCount = 0;
  
  if(bDeviceState == CONFIGURED)
  {
    if (FrameCount++ == VCOMPORT_IN_FRAME_INTERVAL)
    {
      /* Reset the frame counter */
      FrameCount = 0;
      
      /* Check the data to be sent through IN pipe */
      //Handle_USBAsynchXfer();
    }
  }  
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

