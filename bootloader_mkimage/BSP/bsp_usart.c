#include "bsp_usart.h" 
#include "ring_buffer.h"
 


extern UART_HandleTypeDef huart4;



static ring_buffer rx_buf;

void uart4_init(void)
{
  
 //hal库已经做了初始化	
	ring_buffer_init(&rx_buf);
}





/*****************************************************
*function:	写字符文件函数
*param1:	输出的字符
*param2:	文件指针
*return:	输出字符的ASCII码
******************************************************/
 int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart4, (uint8_t*)&ch, 1,0xff);
	return ch;
}





/*****************************************************
*function:	读字符文件函数
*param1:	文件指针
*return:	读取字符的ASCII码
******************************************************/
 int fgetc(FILE *f)
{
    uint8_t ch = 0;
    HAL_UART_Receive(&huart4, (uint8_t*)&ch, 1, 10);
    return (int)ch;
}


/*
 * 添加如下代码，则不需要在工程设置中勾选Use MicroLIB
*/

//#pragma import(__use_no_semihosting)
// 
//struct __FILE
//{
//	int a;
//};
// 
//FILE __stdout;
//FILE __stdin;
//void _sys_exit(int x)
//{
//	
//}




void bsp_debug_uart4_init(void)
{
  uart4_init();
	
	printf("-----------------boot+app experiment-------------------------\r\n");
	
}


void putstr(const char *str)
{
	while (*str)
	{
		putchar(*str);
		str++;
	}
}



int getchar2(void)
{
#if 0
	USART_TypeDef *usart1 = (USART_TypeDef *)0x40013800;
	unsigned int status;
	// while ((usart1->SR & (1<<5)) == 0);
	do {
		status = usart1->SR;
		if (status & (1<<3))
		{
			while (1);
		}
				
	} while ((status & (1<<5)) == 0);
	
	return usart1->DR;
#else
	unsigned char c;
   FILE * ch;
	
//	while (0 != ring_buffer_read(&c, &rx_buf));
	c=fgetc(ch);
	
	return c;
#endif
}

int getchar_nowait(void)
{
#if 0
	USART_TypeDef *usart1 = (USART_TypeDef *)0x40013800;
	unsigned int status;
	// while ((usart1->SR & (1<<5)) == 0);
	do {
		status = usart1->SR;
		if (status & (1<<3))
		{
			while (1);
		}
				
	} while ((status & (1<<5)) == 0);
	
	return usart1->DR;
#else
	unsigned char c;
	
	if (0 == ring_buffer_read(&c, &rx_buf))
		return -1;
	
	return c;
#endif
}


//int putchar(int c)
//{
//	USART_TypeDef *usart1 = (USART_TypeDef *)0x40013800;
//	while ((usart1->SR & (1<<7)) == 0);
//	usart1->DR = c;
//	
//	return c;
//}

//void putstr(const char *str)
//{
//	while (*str)
//	{
//		putchar(*str);
//		str++;
//	}
//}

void putdatas(const char *datas, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		putchar(datas[i]);
	}
}


const unsigned char hex_tab[]={'0','1','2','3','4','5','6','7',\
		                 '8','9','a','b','c','d','e','f'};

void puthex(unsigned int val)
{
	/* val: 0x12345678 */
	int i;
	int index;
	putstr("0x");
	for (i = 7; i >=0 ; i--)
	{
		index = (val >> (i*4)) & 0xf;
		putchar(hex_tab[index]);
	}
}




