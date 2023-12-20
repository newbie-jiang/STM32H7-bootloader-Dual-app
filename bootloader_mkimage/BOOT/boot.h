#ifndef __BOOT_H
#define __BOOT_H

#include "stdint.h"

typedef uint32_t  __be32;

typedef struct {
	uint8_t jump_time;          /* 跳转启动时间 */
	uint8_t which_app_running;  /* 选择app启动  APP1/APP2*/
	uint8_t sys_parameter_flag; /* 第一次使用QSPI检查标志位 */
	uint8_t debug_flag;         /* 打印调试开启标志位  1开启调试 0关闭调试*/
}BOOT_SYS_config; /* 系统配置参数 */



typedef struct{
uint32_t app1_size;
uint32_t app2_size;
}APP_SIZE; /* app大小(字节) */



#define IH_NMLEN	32	/* Image Name Length		*/
typedef struct image_header {
	__be32		ih_magic;	/* Image Header Magic Number	*/
	__be32		ih_hcrc;	/* Image Header CRC Checksum	*/
	__be32		ih_time;	/* Image Creation Timestamp	*/
	__be32		ih_size;	/* Image Data Size		*/
	__be32		ih_load;	/* Data	 Load  Address		*/
	__be32		ih_ep;		/* Entry Point Address		*/
	__be32		ih_dcrc;	/* Image Data CRC Checksum	*/
	uint8_t		ih_os;		/* Operating System		*/
	uint8_t		ih_arch;	/* CPU architecture		*/
	uint8_t		ih_type;	/* Image Type			*/
	uint8_t		ih_comp;	/* Compression Type		*/
	uint8_t		ih_name[IH_NMLEN];	/* Image Name		*/
} image_header_t,*pimage_header_t;



void  jump_to_qspi_flash(void);



#endif
