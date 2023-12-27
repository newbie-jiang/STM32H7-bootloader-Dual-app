#ifndef __CMD_INTERFACE_H__
#define __CMD_INTERFACE_H__


#include "crc.h"
#include "stm32h7xx.h"
#include "types.h"
#include "bsp_usart.h"


/************************************ falsh²Ù×÷ *************************************/
struct flash_ops {
  char *name;
  unsigned int (*read)(unsigned char *buf, unsigned int offset, unsigned int size);
  unsigned int (*write)(unsigned char *buf, unsigned int offset, unsigned int size);
  unsigned int (*erase)(unsigned int offset, unsigned int size);
};
struct flash_ops *get_flash(void);
/*************************************************************************************/

typedef struct sys_parament{
  uint32_t para1;
	uint32_t para2;
  uint32_t para3;
}sys_parament,*psys_parament;




int interface_reboot(int argc, char **argv);
int interface_go(int argc, char **argv);
int interface_ram(int argc, char **argv);
int interface_hardware_crc32( void * StartAddress, uint32_t size);


#endif //__CMD_INTERFACE_H__
