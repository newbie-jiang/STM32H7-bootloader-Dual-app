
#ifndef _UART_H
#define _UART_H
void uart_init(void);
int getchar(void);
int putchar(char c);
void putstr(const char *str);
void puthex(unsigned int val);
#endif
