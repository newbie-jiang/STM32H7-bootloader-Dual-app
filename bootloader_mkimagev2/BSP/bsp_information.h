#ifndef __BSP_INFORMATION_H
#define __BSP_INFORMATION_H

#include "stdint.h"
#include "stm32h7xx.h"

 uint32_t get_stm32h7_SysClockFreq(void);
 uint32_t get_stm32h7_HCLKFreq(void);
 uint32_t get_stm32h7_PCLK1Freq(void);
 uint32_t get_stm32h7_PCLK2Freq(void);
 
 typedef struct stm32h7_id{
   uint32_t stm32h7_id_1;
	 uint32_t stm32h7_id_2;
	 uint32_t stm32h7_id_3;
 }stm32h7_id;



#endif /*__BSP_INFORMATION_H*/

