#ifndef _API_SDRAM_H_
#define _API_SDRAM_H_

#include "stdint.h"
#include "stdbool.h"



typedef struct sdram_driver{
  uint32_t (*sdram_init)(void);
  uint32_t (*sdram_config)(void);

}sdram_driver;

extern sdram_driver ssdram_driver;

uint32_t tets_sdram_driver(sdram_driver * psdram_driver);







#endif /* _API_SDRAM_H_ */



