#ifndef _API_INFORMATION_H_
#define _API_INFORMATION_H_

#include "stdint.h"
#include "stdbool.h"


/********************************************************************************/
/* get chip clock Freq information */
typedef struct api_clock_info{
  uint32_t (*get_SysClockFreq)(void);
  uint32_t (*get_HCLKFreq)(void);
  uint32_t (*get_PCLK1Freq)(void);
  uint32_t (*get_PCLK2Freq)(void);
}api_clock_info;

extern api_clock_info sapi_clock_info;

void test_clock_info(api_clock_info * papi_clock_info);

/********************************************************************************/


/********************************************************************************/
/* get chip id information */
typedef enum chip_id_which{
	 id_0 = 0,
	 id_1 ,
	 id_2 ,
}chip_id_which;



 typedef struct api_chip_id_info{

  uint32_t (*get_chipid)(chip_id_which echip_id_which);
	 
}api_chip_id_info;
 
extern api_chip_id_info sapi_chip_id_info;
 
void test_chip_id_info();

/********************************************************************************/



/********************************************************************************/
/* get chip falsh size information */
typedef struct api_get_falsh_size{
	
 uint32_t (*get_falsh_size)(void);
	
}api_get_falsh_size;

extern api_get_falsh_size sapi_get_falsh_size;

void test_api_get_falsh_size(void);


/********************************************************************************/


#endif /* _API_INFORMATION_H_ */
