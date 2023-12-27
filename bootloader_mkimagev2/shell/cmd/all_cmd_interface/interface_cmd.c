#include "interface_cmd.h"
#include "stdio.h"
#include "boot.h"

extern void jump_app(uint32_t base_address);

//软件复位
int interface_reboot(int argc, char **argv)
{
   NVIC_SystemReset();     // 复位
	 return 0;
}


//跳转函数
int interface_go(int argc, char **argv)
{
	jump_app(0x24000000);
	printf(" go go go \r\n");
	
   
	 return 0;
}


//ram操作
int interface_ram(int argc, char **argv)
{
   
	 return 0;
}


//硬件crc32
int interface_hardware_crc32( void * StartAddress, uint32_t size)
{
   size_t hard_crc32Result = HAL_CRC_Calculate(&hcrc,( void *)StartAddress,size);
   hard_crc32Result ^= 0xffffffffU;
	 return hard_crc32Result;
}










