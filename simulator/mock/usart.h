#ifndef __USART_H__
#define __USART_H__

#include <stdint.h>

#define USART1 1


void usart_send_blocking (uint32_t usart, uint16_t data);


#endif
