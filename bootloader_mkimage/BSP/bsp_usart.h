
#ifndef __USART_H
#define __USART_H

#include "usart.h" 
#include "stdio.h"

void uart4_init(void);

void uart1_dma_init(void);

void bsp_debug_uart4_init(void);
void putstr(const char *str);

#endif /*__USART_H*/

