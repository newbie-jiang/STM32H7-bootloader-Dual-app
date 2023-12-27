#include "bsp_information.h" 


 
 uint32_t get_stm32h7_SysClockFreq(void){
    return HAL_RCC_GetSysClockFreq();
 }
 
  uint32_t get_stm32h7_HCLKFreq(void){
    return HAL_RCC_GetHCLKFreq();
 }
	
  uint32_t get_stm32h7_PCLK1Freq(void){
    return HAL_RCC_GetPCLK1Freq();
 }
	
  uint32_t get_stm32h7_PCLK2Freq(void){
    return HAL_RCC_GetPCLK2Freq();
 }
	
 
 




