#include "bsp_usart.h" 
//#include "ring_buffer.h"
 


extern UART_HandleTypeDef huart4;





void uart4_init(void)
{
  
 //hal库已经做了初始化	
	
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







