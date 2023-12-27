#include "bsp_usart.h" 
//#include "ring_buffer.h"
 #include "stdio.h"


extern UART_HandleTypeDef huart4;



//static ring_buffer rx_buf;

void uart4_init(void)
{
  
 //hal库已经做了初始化	
//	ring_buffer_init(&rx_buf);
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


#ifndef __MICROLIB

#pragma import(__use_no_semihosting)
 
struct __FILE
{
	int a;
};
 
FILE __stdout;
FILE __stdin;
void _sys_exit(int x)
{
	
}

#endif




void bsp_debug_uart4_init(void)
{
  uart4_init();
	
}


void putstr(const char *str)
{
	while (*str)
	{
		putchar(*str);
		str++;
	}
}



int my_getchar(void)
{
  unsigned char c;
  FILE * ch;
	c=fgetc(ch);
	return c;
}


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




