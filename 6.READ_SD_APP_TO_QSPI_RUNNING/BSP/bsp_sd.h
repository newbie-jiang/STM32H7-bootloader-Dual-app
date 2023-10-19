
#ifndef __SDCARD_H
#define __SDCARD_H

#include "stdio.h" 
#include "sdmmc.h" 
#include "string.h" 
#include "fatfs.h" 



#define  FATFS_DEBUG


void SD_EraseTest(void);
void SD_Write_Read_Test(void);
void get_sd_informatization(void);
void fatfs_test(void);


void sd_application(void);


/*文件操作函数封装    使用格式化 先格式化SD卡再挂载*/  
 FRESULT mkfs_sdcard(void);/*格式化sd卡*/
 int mount_sd(void);/*挂载sd卡*/
 void bsp_sd_init(void);/*挂载sd卡*/
 
 uint8_t createDirectory(const char* path);/*创建目录*/
 uint8_t create_a_file(const char* folderPath, const char* filename);//创建文件
 uint8_t writeToFile(const char *filePath, const void *buffer, uint32_t length);/*写文件*/
 int readFileContent(const char* filePath, void* buffer, int bufferSize, DWORD offset);/*读文件函数*/


#endif /*__SDCARD_H*/
