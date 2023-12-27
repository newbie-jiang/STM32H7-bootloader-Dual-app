#ifndef   __BOOT_LOADER
#define   __BOOT_LOADER

#include "stdint.h"
#include "boot_head.h"



typedef struct{
	struct image_header simage_head; //头部64字节
	uint32_t sys_boot_crc32;  //4字节
	uint8_t  sys_boot_delay;  //4字节
}sys_boot_parament;














#endif /* __BOOT_LOADER  */
