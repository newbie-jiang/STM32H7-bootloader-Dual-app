#include "bsp_key.h" 
#include "stm32h7xx.h" 



/*按键初始化*/
 void key_init(void)
 {
	 /*解初始化*/
	HAL_GPIO_DeInit(__KEY_Port,__KEY_Pin);
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __KEY_CLK_ENABLE;
  GPIO_InitStruct.Pin = __KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(__KEY_Port, &GPIO_InitStruct);
 } 
 