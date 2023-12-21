
#ifndef _UART_H
#define _UART_H
void uart_init(void);

#undef getchar
int getchar2(void);

#undef putchar
int putchar(int c);

void putstr(const char *str);
void puthex(unsigned int val);
void putdatas(const char *datas, int len);

#endif
