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
  
  /* Get the received data buffer and update the counter */
  //USB_Rx_Cnt = USB_SIL_Read(EP3_OUT, frame_buffer);
  
  /* USB data will be immediately processed, this allow next USB traffic being 
  NAKed till the end of the USART Xfer */
  /*if (frame_buffer[254] = 1 && frame_buffer[255] == 2){
    GPIOC->ODR = 0xffff;
    //sleep(10);
    GPIOC->ODR = 0x0000;
  }*/
  
  //not used
  //USB_To_USART_Send_Data(frame_buffer, USB_Rx_Cnt);
  
  if (GetENDPOINT(ENDP3) & EP_DTOG_TX) {
    /*read from ENDP1_BUF0Addr buffer*/
    data_len = GetEPDblBuf0Count(ENDP3);
    PMAToUserBufferCopy((uint8_t*)& frame_buffer[512*current_usb_frame], ENDP3_BUF0Addr, data_len);
    //if (frame_buffer[0] == 0){
    if (data_len > 0){
      //for(int i = 512*current_usb_frame;i<512*(current_usb_frame+1);i++){
        //frame_buffer[i] = 1;
      //}
      if (frame_buffer[255] == 255){
        GPIOC->ODR = 0xffff;
        sleep(10);
        GPIOC->ODR = 0x0000;
      }
    }
  } else {
    /*read from ENDP1_BUF1Addr buffer*/
    data_len = GetEPDblBuf1Count(ENDP3);
    PMAToUserBufferCopy(& frame_buffer[512*current_usb_frame], ENDP3_BUF1Addr, data_len);
  }

  if (++current_usb_frame >= NUM_USB_FRAMES){
    current_usb_frame = 0;
  }

  FreeUserBuffer(ENDP3, EP_DBUF_OUT);

#ifndef STM32F10X_CL
  /* Enable the receive of data on EP3 */
  //SetEPRxValid(ENDP3);
#endif /* STM32F10X_CL */
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

