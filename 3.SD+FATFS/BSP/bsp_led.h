#ifndef __LED_H
#define __LED_H

#include "gpio.h"

#include "bsp_tick.h" 

 /*开关电平状态*/
 #define      LED_ON              GPIO_PIN_RESET
 #define      LED_OFF             GPIO_PIN_SET
 
 

 /*引脚配置*/
 #define  __LED_R_Port             GPIOC
 #define  __LED_R_Pin              GPIO_PIN_15 
 #define  __LED_R_CLK_ENABLE       do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)
 
 
 #define  __LED_B_Port             GPIOI
 #define  __LED_B_Pin              GPIO_PIN_8 
 #define  __LED_B_CLK_ENABLE       do{ __HAL_RCC_GPIOI_CLK_ENABLE(); }while(0)
 
 
 #define  TEST_IO_Port             GPIOB
 #define  TEST_IO_Pin              GPIO_PIN_2 
 #define  TEST_IO_CLK_ENABLE       do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)



/*控制开关*/
 #define  LED_R(x)   do{ x ? \
                      HAL_GPIO_WritePin(__LED_R_Port, __LED_R_Pin, LED_ON) : \
                      HAL_GPIO_WritePin(__LED_R_Port, __LED_R_Pin, LED_OFF); \
                  }while(0)      

 #define  LED_B(x)   do{ x ? \
                      HAL_GPIO_WritePin(__LED_B_Port, __LED_B_Pin, LED_ON) : \
                      HAL_GPIO_WritePin(__LED_B_Port, __LED_B_Pin, LED_OFF); \
                  }while(0)     

/* 控制翻转 */
#define LEDR_TOGGLE()   do{ HAL_GPIO_TogglePin(__LED_R_Port, __LED_R_Pin); }while(0)        /* 翻转LED_R */
#define LEDB_TOGGLE()   do{ HAL_GPIO_TogglePin(__LED_B_Port, __LED_B_Pin); }while(0)        /* 翻转LED_B */


 
 #define TEST_GPIO_TOGGLE  do{ HAL_GPIO_TogglePin(TEST_IO_Port, TEST_IO_Pin); }while(0)
 
 #define TEST_GPIO(x)      do{ x ? \
                             HAL_GPIO_WritePin(TEST_IO_Port, TEST_IO_Pin, GPIO_PIN_SET) : \
                             HAL_GPIO_WritePin(TEST_IO_Port, TEST_IO_Pin, GPIO_PIN_RESET); \
                             }while(0)     



/*初始化*/
void led_init(void);
void bsp_led_init(void);

#endif /*__LED_H*/
