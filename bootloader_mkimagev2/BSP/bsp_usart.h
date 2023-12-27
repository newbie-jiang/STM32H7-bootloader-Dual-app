
#ifndef __BSP_USART_H
#define __BSP_USART_H

#include "usart.h" 
//#include "stdio.h"

void uart4_init(void);

void uart1_dma_init(void);

void bsp_debug_uart4_init(void);
//void putstr(const char *str);

void putstr(const char *str);
int my_getchar(void);
#endif /*__BSP_USART_H*/

