#ifndef _API_UART_H_
#define _API_UART_H_

#include "stdint.h"
#include "stdbool.h"



typedef struct uart_driver{
	
  uint32_t (*uart_init)(uint32_t baud);
  uint32_t (*uart_init_interrupt)(uint32_t baud);
	
	uint32_t (*rs232_enable_hard_fallow_control)(void);
  uint32_t (*rs475_enable_hard_fallow_control)(void);
	
	uint32_t (*uart_send)(uint8_t *data, uint32_t size);
	uint32_t (*uart_receive)(uint8_t *buffer, uint32_t size);
	
	uint32_t (*uart_send_it)(uint8_t *data, uint32_t size);
	uint32_t (*uart_receive_it)(uint8_t *buffer, uint32_t size);
	
	uint32_t (*uart_send_dma)(uint8_t *data, uint32_t size);
	uint32_t (*uart_receive_dma)(uint8_t *buffer, uint32_t size);
	
	uint32_t (*printf_redirect)(void);
	uint32_t (*put_string)(void);
	

	uint32_t (*uart_disable)(void);
	
	uint32_t (*init_cjson)(void);
	
	uint32_t (*init_shell)(void);
	
	uint32_t (*init_zmodem)(void);
	
}uart_driver;

extern uart_driver suart_driver;
uint32_t test_uart_driver(uart_driver * puart_driver);

#endif /* _API_UART_H_ */