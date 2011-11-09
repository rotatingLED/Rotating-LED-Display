/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.3.0
* Date               : 21-March-2011
* Description        : Descriptors for Virtual Com Port Demo
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

/* USB Standard Device Descriptor */
const uint8_t Virtual_Com_Port_DeviceDescriptor[] =
  {
    0x12,   /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
    0x00,
    0x02,   /* bcdUSB = 2.00 */
    0x00,   /* bDeviceClass: CDC -> originally 02, but changed */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    0x40,   /* bMaxPacketSize0 */
    0xff,
    0xff,   /* idVendor = 0x0483 -> 0xffff added by david halter -> undefined */
    0x40,
    0x57,   /* idProduct = 0x5740 */
    0x00,
    0x02,   /* bcdDevice = 2.00 */
    1,              /* Index of string descriptor describing manufacturer */
    2,              /* Index of string descriptor describing product */
    3,              /* Index of string descriptor describing the device's serial number */
    0x01    /* bNumConfigurations */
  };

const uint8_t Virtual_Com_Port_ConfigDescriptor[] =
  {
    /*Configuration Descriptor*/
    0x09,   /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
    VIRTUAL_COM_PORT_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */
    0x00,
    0x02,   /* bNumInterfaces: 2 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
    0xC0,   /* bmAttributes: self powered */
    0x32,   /* MaxPower 0 mA */
    /*Interface Descriptor*/
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   // bInterfaceNumber: Number of Interface 
    0x00,   // bAlternateSetting: Alternate setting 
    0x01,   // bNumEndpoints: One endpoints used 
    0x00,         // bInterfaceClass: HID -> 03 
    0x00,         // bInterfaceSubClass : 1=BOOT, 0=no boot 
    0x00,         // nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse 
    0x00,   // iInterface: 

    /*
    // USB Speaker Audio Streaming Interface Descriptor 
    0x07,  // bLength 
    0x25,      // bDescriptorType 
    0x01,              // audio-streaming-general bDescriptorSubtype 
    0x01,                                 // bTerminalLink 
    0x01,                                 // bDelay 
    0x02,                                 // wFormatTag AUDIO_FORMAT_PCM8  0x0002
    0x00,
    // 07 byte

    // USB Speaker Audio Type I Format Interface Descriptor 
    0x0B,                                 // bLength 
    0x25,      // Audio_interface_descriptor_type bDescriptorType 
    0x02,          // audio_streaming-format-type bDescriptorSubtype 
    0x01,                  // bFormatType 
    0x01,                                 // bNrChannels 
    0x01,                                 // bSubFrameSize 
    8,                                    // bBitResolution 
    0x01,                                 // bSamFreqType 
    0xF0,                                 // tSamFreq 22000 = 0x55F0 
    0x55,
    0x00,
    // 11 byte
    
    */
    //Header Functional Descriptor
    0x05,   // bLength: Endpoint Descriptor size 
    0x24,   // bDescriptorType: CS_INTERFACE 
    0x00,   // bDescriptorSubtype: Header Func Desc 
    0x10,   // bcdCDC: spec release number 
    0x01,
    //Call Management Functional Descriptor
    0x05,   // bFunctionLength 
    0x24,   // bDescriptorType: CS_INTERFACE 
    0x01,   // bDescriptorSubtype: Call Management Func Desc 
    0x00,   // bmCapabilities: D0+D1 
    0x01,   // bDataInterface: 1 
    //ACM Functional Descriptor
    0x04,   // bFunctionLength 
    0x24,   // bDescriptorType: CS_INTERFACE 
    0x02,   // bDescriptorSubtype: Abstract Control Management desc 
    0x02,   // bmCapabilities 
    //Union Functional Descriptor
    0x05,   // bFunctionLength 
    0x24,   // bDescriptorType: CS_INTERFACE 
    0x06,   // bDescriptorSubtype: Union func desc 
    0x00,   // bMasterInterface: Communication class interface 
    0x01,   // bSlaveInterface0: Data Class Interface 

    //Endpoint 2 Descriptor
    0x07,   // bLength: Endpoint Descriptor size 
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x82,   /* bEndpointAddress: (IN2) */
    0x03,   /* bmAttributes: Interrupt */
    VIRTUAL_COM_PORT_INT_SIZE,      /* wMaxPacketSize: */
    0x00,
    0xFF,   /* bInterval: */
    /*Data class interface descriptor*/
    0x09,   /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
    0x01,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: Two endpoints used */
    0x0A,   /* bInterfaceClass: CDC */
    0x00,   /* bInterfaceSubClass: */
    0x00,   /* bInterfaceProtocol: */
    0x00,   /* iInterface: */
    /*Endpoint 3 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x03,   /* bEndpointAddress: (OUT3) */
    USB_ENDPOINT_TYPE_ISOCHRONOUS,   /* bmAttributes: Bulk */
    0x00,   /*USB_OUT_DATA_SIZE,             /* wMaxPacketSize: */
    0x04,
    0x01,   /* bInterval: ignore for Bulk transfer */
    //0x00,   /* bRefresh */
    //0x00,   /* bSynchAddress */

    /*Endpoint 1 Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    0x81,   /* bEndpointAddress: (IN1) */
    USB_ENDPOINT_TYPE_BULK,   /* bmAttributes: Bulk */
    VIRTUAL_COM_PORT_DATA_SIZE,             /* wMaxPacketSize: */
    0x00,
    0x00,    /* bInterval */

    0x07,   // bLength 
    0x25,       // (audio_endpoint) bDescriptorType 
    0x01,               // bDescriptor 
    0x00,                                 // bmAttributes 
    0x00,                                 // bLockDelayUnits 
    0x00,                                 // wLockDelay 
    0x00,
  };

/* USB String Descriptors */
const uint8_t Virtual_Com_Port_StringLangID[VIRTUAL_COM_PORT_SIZ_STRING_LANGID] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

const uint8_t Virtual_Com_Port_StringVendor[VIRTUAL_COM_PORT_SIZ_STRING_VENDOR] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_VENDOR,     /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer: "STMicroelectronics" */
    'Z', 0, 'H', 0, 'A', 0, 'W', 0
  };

const uint8_t Virtual_Com_Port_StringProduct[VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    /* Product name: "LED controller" */
    'L', 0, 'E', 0, 'D', 0, ' ', 0, 'c', 0, 'o', 0, 'n', 0, 't', 0,
    'o', 0, 'l', 0, 'l', 0, 'e', 0, 'r', 0


  };

uint8_t Virtual_Com_Port_StringSerial[VIRTUAL_COM_PORT_SIZ_STRING_SERIAL] =
  {
    VIRTUAL_COM_PORT_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  };

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
