#ifndef __SERIAL__h__
#define __SERIAL__h__

void SerialPutChar(unsigned char c);
void SerialPutString(unsigned char *s);
void USART1_Init(void);
void SerialPrintf (const char *fmt, ...);

#endif
