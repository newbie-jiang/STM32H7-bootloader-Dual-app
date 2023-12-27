#ifndef __CONFIG_H
#define __CONFIG_H


/******************  APP1 ******************************/
/* QSPI APP1 映射基地址 */
#define     QSPI_APP1_MM_BASE_ADDRESS       0x90000000UL

/* QSPI APP1 非映射基地址*/
#define     QSPI_APP1_BASE_ADDRESS          0x00000000UL

/* SDRAM 缓存APP1的基地址*/
#define     SDRAM_TARGET1_ADDR              0xC0000000UL

/* 读取APP1文件名app1.bin */
#define     APP1_FILE_NAME      "app1_head.bin"
/********************************************************/

/******************  APP2   ****************************/
/* QSPI APP2 映射基地址 */
#define     QSPI_APP2_MM_BASE_ADDRESS       0x90400000UL  

/* QSPI APP2 非映射基地址 */
#define     QSPI_APP2_BASE_ADDRESS          0x00400000UL  

/* SDRAM 缓存APP2的基地址*/
#define     SDRAM_TARGET2_ADDR              0xC1000000UL

/* 读取APP2文件名app2.bin */
#define     APP2_FILE_NAME       "app2_head.bin"
/***********************************************************/

/* SD卡与SDRAM数据对比缓冲区*/
#define     BLOCK_SIZE                1024   // 1KB

/* ************长短按键时间ms************************/
#define     SHORT_PRESS_TIME          20   
#define     LONG_PRESS_TIME           1500  
/**************************************************/


/****************上电QSPI默认参数配置***************/
/* 参数标志位 QSPI地址(最后一个扇区4k) */
#define     SYS_PARAMENT_ADDRESS      0x7FF000 

/* 默认参数配置 (仅第一次烧录有效) */
#define      JUMP_TIME                 0x03
#define      WHICH_APP                 0x01
#define      PARA_FLAG                 0x01
#define      DEBUG_FLAG                0x01

/* 重置系统参数(注:若要更改默认参数配置并生效,需启用该宏开关1次 烧录之后关闭该宏再次烧录 防止每次主动清除)*/
//#define      CLEAN_SYS_PARAMENT        

/***************************************************/
#define      AHEAD_SP        0x24
#define      AHEAD_PC        0x90
/***************************************************/
/* 缓冲区大小（内部flash）*/
#define BUFFER_SIZE              1024
/* QSPI页大小 */
#define W25Q64JV_SECTOR_SIZE     0x1000   
/***************************************************/


#endif /* __CONFIG_H */

