#include "bsp_tick.h" 


/*Hal库延时函数封装ms*/
void delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}

/*获取系统时间ms*/
uint32_t get_tick_ms(void)
{
 return HAL_GetTick();
}







