#ifndef __BOOT_H
#define __BOOT_H

#include "stdint.h"

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





void  jump_to_qspi_flash(void);



#endif
