#include "boot.h"

#include "bsp_qspi.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_sd.h" 
#include "bsp_sdram.h" 
#include "bsp_key.h" 

#include "config.h" 


uint32_t calculate_crc32(const void *data, size_t size);
  

void jump_app(uint32_t base_address);
void display_logo(void);
int read_app_and_jump_running(void);
void get_parament_which_boot_start(uint8_t time, uint8_t which_app, uint8_t debug);
int which_jump_to_app1_or_app2(uint8_t para_flag);
 
int Read_app_in_sd_card(void);



 typedef  void (*pFunction)(void);
 pFunction JumpToApplication;

 FILINFO fileInfo;
 APP_SIZE Sapp_size={0,0};
 APP_SIZE * Papp_size = &Sapp_size;

 BOOT_SYS_config sys_parameter;
 BOOT_SYS_config * psys_parameter = &sys_parameter;	
 
 
 
 
 

void  jump_to_qspi_flash(void)
{
	read_app_and_jump_running();
}



/* 以防万一 ，在最后开启qspi时钟 */
void enable_qspi_clk(void)
{
  __HAL_RCC_QSPI_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
	
  __HAL_RCC_FMC_CLK_ENABLE(); /* 坑死我了，这个时钟必须开 没细看这个时钟与QSPI关系  */
}






/* 确保一个干净的bootloader 我们需要解初始化用到的所有外设以及中断  只保留QSPI，在APP中就不需要动QSPI了 */
/* 避坑: 在解初始化的时候注意不要将QSPI的时钟也解初始化了 有的外设与QSPI一个时钟，解初始化之后就不工作了
   如果不正常工作，解初始化之后 再读看看QSPI是否正常 */
void DeInit_all_bsp_and_IRQ(void)
{ 
   SCB_DisableICache();		// 关闭ICache
	 SCB_DisableDCache();		// 关闭Dcache
 	
	 SysTick->CTRL = 0;		// 关闭SysTick
	 SysTick->LOAD = 0;		// 清零重载值
	 SysTick->VAL = 0;		// 清零计数值
	
	/* 关闭所有中断，清除所有中断挂起标志 */
    for (int i = 0; i < 8; i++)
    {
        NVIC->ICER[i] = 0xFFFFFFFF;
        NVIC->ICPR[i] = 0xFFFFFFFF;
    } 
	
	
	 /* 解初始化 SD卡 */
	 HAL_SD_MspDeInit(&hsd1);
		
		
	 /* 解初始化 SDRAM */
//	 HAL_SDRAM_MspDeInit(&hsdram1);
		
	 
	 /*解初始化LED*/ 
	 HAL_GPIO_DeInit(LED_R_GPIO_Port,LED_R_Pin);
	 HAL_GPIO_DeInit(LED_B_GPIO_Port,LED_R_Pin);
		
	 /*解初始化 SD_CAP*/
	 HAL_GPIO_DeInit(SD_CAP_GPIO_Port,SD_CAP_Pin);
		
		
	 /*解初始化usart */
   HAL_UART_MspDeInit(&huart4);
	 
	
	/*设置所有时钟默认状态恢复HSI*/
//	 HAL_RCC_DeInit();
	 
	 /* 为防止qspi时钟 被初始化 */
//	 enable_qspi_clk();
//    HAL_SDRAM_DeInit(&hsdram1);
}


 
 

/* 函数功能：跳转到 app */
void jump_app(uint32_t base_address)
{
//    pFunction JumpToApplication;

    /* 取消初始化所有外设和中断 */
    DeInit_all_bsp_and_IRQ();
    
    /* 确保CPU不响应任何中断。注意：在应用程序中，您需要手动使用 __enable_irq(); 来启用中断 */
    __disable_irq();
    
    /* 设置跳转地址 */
    JumpToApplication = (pFunction) (*(__IO uint32_t*) (base_address + 4));
    
    /* 设置主堆栈指针 */
    __set_MSP(*(__IO uint32_t*) base_address);
    
    /* 执行跳转 */
	
	  SCB->VTOR = 0x24000000;
	
//	  memcmp();
	
    JumpToApplication();
}

 

 /*     logo 生成网站：http://patorjk.com/software/taag/       */
 
 void display_logo(void)
{
	                                                                                                                                                                   
  printf("'  BBBBBBBBBBBBBBBBB        OOOOOOOOO          OOOOOOOOO     TTTTTTTTTTTTTTTTTTTTTTT\r\n");
  printf("'  B::::::::::::::::B     OO:::::::::OO      OO:::::::::OO   T:::::::::::::::::::::T\r\n");
  printf("'  B::::::BBBBBB:::::B  OO:::::::::::::OO  OO:::::::::::::OO T:::::::::::::::::::::T\r\n");
  printf("'  BB:::::B     B:::::BO:::::::OOO:::::::OO:::::::OOO:::::::OT:::::TT:::::::TT:::::T\r\n");
  printf("'    B::::B     B:::::BO::::::O   O::::::OO::::::O   O::::::OTTTTTT  T:::::T  TTTTTT\r\n");
  printf("'    B::::B     B:::::BO:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::BBBBBB:::::B O:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B:::::::::::::BB  O:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::BBBBBB:::::B O:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::B     B:::::BO:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::B     B:::::BO:::::O     O:::::OO:::::O     O:::::O        T:::::T        \r\n");
  printf("'    B::::B     B:::::BO::::::O   O::::::OO::::::O   O::::::O        T:::::T        \r\n");
  printf("'  BB:::::BBBBBB::::::BO:::::::OOO:::::::OO:::::::OOO:::::::O      TT:::::::TT      \r\n");
  printf("'  B:::::::::::::::::B  OO:::::::::::::OO  OO:::::::::::::OO       T:::::::::T      \r\n");
  printf("'  B::::::::::::::::B     OO:::::::::OO      OO:::::::::OO         T:::::::::T      \r\n");
  printf("'                                                                                   \r\n");
	printf("'                                                                                   \r\n");
	printf("'  www.newbie.pics                                                                  \r\n");
	printf("'                                                                                   \r\n");
	printf("'                                                                                   \r\n");
	printf("'                                                                                   \r\n");
	printf("'                                                                                   \r\n");

}



/*将SD数据读取至sdram
  将SD数据读取至1k缓存,与sdram存放区域的每个字节进行校验  循环读取逐字节校验
*/

/* 
 * 选取app读取到 sdram 
 * 1k 缓冲逐字节校验
 */
int read_sd_data_to_sdram(uint8_t app_which)
{
	  FRESULT res;
    uint32_t bytesRead;
	  
	
	  if(app_which==1) /* 选择app1 */
	  {
		     uint8_t* sdram_ptr1 = (uint8_t*)SDRAM_TARGET1_ADDR;  // 指向SDRAM的指针
         uint8_t  temp_buffer1[BLOCK_SIZE];  // 临时缓冲区
		 // 获取文件信息
				res = f_stat(APP1_FILE_NAME, &fileInfo);
				if (res != FR_OK)
				{
						printf("  Error getting file info.\r\n"); /* 文件不存在*/
						return -1;
				}
				else
				{
					  Sapp_size.app1_size=fileInfo.fsize;
						printf("  The size of the file  %s is %ld bytes.\r\n", APP1_FILE_NAME,fileInfo.fsize); /*获取文件大小*/
				}

				// 读取整个文件到SDRAM
				bytesRead = readFileContent(APP1_FILE_NAME, sdram_ptr1, fileInfo.fsize+1, 0);
				
				if (bytesRead != fileInfo.fsize)
				{
						printf("  Error reading the entire file.\r\n");
						return -1;
				}

				// 以1KB的块大小进行校验
				for (uint32_t i = 0; i < fileInfo.fsize; i += BLOCK_SIZE)
				{
						// 从文件中读取1KB的块到临时缓冲区
						bytesRead = readFileContent(APP1_FILE_NAME, temp_buffer1, BLOCK_SIZE, i);
						if (bytesRead != BLOCK_SIZE && i + bytesRead != fileInfo.fsize)
						{
								printf("  Error reading the file block to temp buffer.\r\n");
								return -1;
						}

						// 比较SDRAM中的块和临时缓冲区中的数据
						if (memcmp(sdram_ptr1 + i, temp_buffer1, bytesRead) != 0)
						{
								printf("  Data mismatch detected at block starting from byte %d.\r\n", i);
								return -1;
						}
				}
				 //uint8_t* sdram_ptr1 = (uint8_t*)SDRAM_TARGET1_ADDR; 
				 uint32_t app1_crc32=calculate_crc32(sdram_ptr1+64,Sapp_size.app1_size-64);
				 printf("app1_crc32: 0x%08X\r\n", app1_crc32);

				  printf("  Data in SDRAM check the file content success.\r\n");
				  return 0;
	 }else if(app_which==2){
		
		     uint8_t* sdram_ptr2 = (uint8_t*)SDRAM_TARGET2_ADDR;  // 指向SDRAM的指针
         uint8_t  temp_buffer2[BLOCK_SIZE];  // 临时缓冲区
	
			 // 获取文件信息
				res = f_stat(APP2_FILE_NAME, &fileInfo);
				if (res != FR_OK)
				{
						printf("  Error getting file info.\r\n"); /*文件不存在*/
						return -1;
				}
				else
				{   
					  Sapp_size.app2_size=fileInfo.fsize;
						printf("  The size of the file  %s is %ld bytes.\r\n", APP2_FILE_NAME,fileInfo.fsize); /*获取文件大小*/
				}

				// 读取整个文件到SDRAM
				bytesRead = readFileContent(APP2_FILE_NAME, sdram_ptr2, fileInfo.fsize+1, 0);
				
				if (bytesRead != fileInfo.fsize)
				{
						printf("  Error reading the entire file.\r\n");
						return -1;
				}

				// 以1KB的块大小进行校验
				for (uint32_t i = 0; i < fileInfo.fsize; i += BLOCK_SIZE)
				{
						// 从文件中读取1KB的块到临时缓冲区
						bytesRead = readFileContent(APP2_FILE_NAME, temp_buffer2, BLOCK_SIZE, i);
						if (bytesRead != BLOCK_SIZE && i + bytesRead != fileInfo.fsize)
						{
								printf("  Error reading the file block to temp buffer.\r\n");
								return -1;
						}

						// 比较SDRAM中的块和临时缓冲区中的数据
						if (memcmp(sdram_ptr2 + i, temp_buffer2, bytesRead) != 0){
						    printf("  Data mismatch detected at block starting from byte %d.\r\n", i);
								return -1;
						  }
				   }
				 uint32_t app2_crc32=calculate_crc32(sdram_ptr2+64,Sapp_size.app2_size-64);
				 printf("app2_crc32: 0x%08X\r\n", app2_crc32);
				   printf("  Data in SDRAM check the file content success.\r\n");
			return 0;
	  }
    
	return -1;
}



void in_sdram_crc_check(void)
{
	
	
	 /*地址 大小*/
//   calculate_crc32();



}






/*
 *获取固件大小，根据固件大小擦除扇区
 *将sdram中数据写入qspi flash 
 *参数可选 1/2
 */
int write_sdram_data_to_qspi_flash(uint8_t which_app)
{
    if (which_app == 1) {
        uint8_t* sdram_data_ptr1 = (uint8_t*)SDRAM_TARGET1_ADDR+64;
			  uint32_t remaining_bytes1 = Papp_size->app1_size-64; /* APP1大小(字节)*/
       
			 /************************************擦除扇区(按4k擦除)***************************************/
			  uint32_t start_sector_app1 = QSPI_APP1_BASE_ADDRESS; 
        uint32_t end_sector = (remaining_bytes1 + W25Q64JV_SECTOR_SIZE - 1) / W25Q64JV_SECTOR_SIZE - 1;
        uint32_t sectors_need_Erase_num = end_sector - start_sector_app1 + 1;

        printf(" app1 fileInfo.fsize=%d\r\n", remaining_bytes1);
        printf(" app1 sectors need Erase num = %04d\r\n", sectors_need_Erase_num);

        for (uint32_t i = start_sector_app1; i < sectors_need_Erase_num; i++) {
            if (QSPI_W25Qxx_SectorErase(i * W25Q64JV_SECTOR_SIZE) != QSPI_W25Qxx_OK) {
                printf(" qspi erase err!!!\r\n");
                return -1;
            }
            printf(" qspi erase %d sectors success!!!\r\n", i);
        }
				/******************************************************************************************/
				

        if (QSPI_W25Qxx_WriteBuffer(sdram_data_ptr1, 0x0, remaining_bytes1) != QSPI_W25Qxx_OK) {
            return -1;
        }
    } else if (which_app == 2) {
			
        uint8_t* sdram_data_ptr2 = (uint8_t*)SDRAM_TARGET2_ADDR+64;
        uint32_t remaining_bytes2 = Papp_size->app2_size-64;
        /************************************擦除扇区(按4k擦除)***************************************/
        uint32_t start_sector_app2 = QSPI_APP2_BASE_ADDRESS / W25Q64JV_SECTOR_SIZE;
        uint32_t end_sector = start_sector_app2 + (remaining_bytes2 + W25Q64JV_SECTOR_SIZE - 1) / W25Q64JV_SECTOR_SIZE - 1;
        uint32_t sectors_need_Erase_num = end_sector - start_sector_app2 + 1;

        printf(" app2 fileInfo.fsize=%d\r\n", remaining_bytes2);
        printf(" app2 sectors_need_Erase_num = %04d\r\n", sectors_need_Erase_num);

        for (uint32_t i = start_sector_app2; i <= end_sector; i++) {
            if (QSPI_W25Qxx_SectorErase(i * W25Q64JV_SECTOR_SIZE) != QSPI_W25Qxx_OK) {
                printf(" qspi erase err at sector %d!!!\r\n", i);
                return -1;
            }
            printf(" qspi erase sector %d success!!!\r\n", i);
        }
        /******************************************************************************************/
				
        if (QSPI_W25Qxx_WriteBuffer(sdram_data_ptr2, QSPI_APP2_BASE_ADDRESS, remaining_bytes2) != QSPI_W25Qxx_OK) {
            return -1;
        }
    } else {
			 
        return -1; // 参数错误
    }

    return 0;
}

/* 验证QSPI Flash中的数据
 * 参数可选 1/2
 * 返回值 0 :success    -1 :err
 */
int verify_qspi_data(uint8_t which_app)
{
    if (which_app == 1) {
        uint8_t* sdram_data_ptr1 = (uint8_t*)SDRAM_TARGET1_ADDR+64;
        uint8_t* qspi_data_ptr1 = (uint8_t*)QSPI_APP1_MM_BASE_ADDRESS;
        uint32_t remaining_bytes1 = Papp_size->app1_size-64;

        if (memcmp(sdram_data_ptr1, qspi_data_ptr1, remaining_bytes1) != 0) {
            printf(" app1 qspi memorymap data check err!!!\r\n");
            return -1;
        }
        printf(" app1 qspi memorymap data check success!!!\r\n");
				return 0;
    } else if (which_app == 2) {
        uint8_t* sdram_data_ptr2 = (uint8_t*)SDRAM_TARGET2_ADDR+64;
        uint8_t* qspi_data_ptr2 = (uint8_t*)QSPI_APP2_MM_BASE_ADDRESS;
        uint32_t remaining_bytes2 = Papp_size->app2_size-64;

        if (memcmp(sdram_data_ptr2, qspi_data_ptr2, remaining_bytes2) != 0) {
            printf(" app2 qspi memorymap data check err!!!\r\n");
            return -1;
        }
        printf(" app2 qspi memorymap data check success!!!\r\n");
				return 0;
    } else {
        return -1; // 参数错误
    }

//    return 0;
}

	


	

/*  检查qspi中是否有app 这里只检查 前八个字节中是否有类似的sp与pc数据  
     上电后的第一个四字节为sp堆栈首地址：一般为0x2000 xxxx   我们识别0x20这个字节   
	   由于是小端 QSPI中的数据存储形式为0x xxxx 0020 即，我们去读第四个字节，我的sp为0x24000000
	   所以我判断基地址偏移四字节数据为0x24就行   pc一样，0x9000 xxxx  读基地址偏移八字节数据
	   
	   *((base_add+4)==0x24)&&(*(base_add+8)==0x90) 则判定为app存在
*/ 
/* 检查QSPI 中 是否存在app1 app2
   app1存在  return   1 
   app2存在  return   2
   双app存在 return   3
   无app存在 return  -1
*/

/* 检查QSPI 中 app1 app2是否存在 */
int check_qspi_app_exist(void){
 
  uint8_t app1_sp = AHEAD_SP;  
	uint8_t app1_pc = AHEAD_PC;
	uint8_t app2_sp = AHEAD_SP;  
	uint8_t app2_pc = AHEAD_PC;
	
	uint8_t app1_check_buffer[8];
	uint8_t app2_check_buffer[8];
	uint8_t app1_flag=0,app2_flag=0;  /* app存在标志位 0：不存在   1：存在 */
	

	QSPI_W25Qxx_ReadBuffer(app1_check_buffer, QSPI_APP1_BASE_ADDRESS, sizeof(app1_check_buffer));
	QSPI_W25Qxx_ReadBuffer(app2_check_buffer, QSPI_APP2_BASE_ADDRESS, sizeof(app2_check_buffer));
	
	/* 检查app1是否存在 */
	if((app1_check_buffer[3]==app1_sp)&&(app1_check_buffer[7]==app1_pc)){
		app1_flag = 1;
	}
	/* 检查app2是否存在*/
	if((app2_check_buffer[3]==app2_sp)&&(app2_check_buffer[7]==app2_pc)){
		app2_flag = 1;
  }
	
	 
	 if (app1_flag && app2_flag) {
    printf(" QSPI EXIST APP1 and APP2 \r\n"); /* 两个app都存在 */
    return 3; //双app存在
    } else if (app1_flag) {
    printf(" QSPI EXIST APP1 \r\n"); /* 只有app1存在 */
    return 1;
    } else if (app2_flag) {
    printf(" QSPI EXIST APP2 \r\n"); /* 只有app2存在 */
    return 2;
    } else {
    printf(" No APP found in QSPI \r\n"); /* 没有app存在 */
    return -1;
    }
		
//		return -1;
	
}








/* 
 * 上电系统参数配置 
 * 未读取到参数则写入（适用于第一次将配置参数写入QSPI）
 * 同步系统参数到 sys_parameter结构体
 * 返回值：  成功读取到参数返回：0    错误： -1
 */
int  sys_parameter_config (void) 
{
	/* buffer[0]跳转时间  buffer[1]APP选择  buffer[2]参数标志位   buffer[3]debug标志位 */
	uint8_t read_sys_parament_buffer[4];  
	
	/* 系统上电默认参数 */
	sys_parameter.jump_time=JUMP_TIME; /* 3s */
	sys_parameter.which_app_running = WHICH_APP;  /* 1:app1  2:app2*/
	sys_parameter.sys_parameter_flag =PARA_FLAG;  /* 配置参数标志位1为存在标志位，0为不存在标志位 */
	sys_parameter.debug_flag = DEBUG_FLAG;
	
	// 获取指向结构体的字节指针
  uint8_t* psys_parameter = (uint8_t*)&sys_parameter;
	
	
	/* 清除系统配置参数 （注：该宏开关仅使用一次，使用后应关闭）*/
	#ifdef CLEAN_SYS_PARAMENT
	   if(QSPI_W25Qxx_SectorErase(SYS_PARAMENT_ADDRESS)!=QSPI_W25Qxx_OK){
			    printf(" Erase QSPI Sector parament err\r\n ");
		      return -1;
		 }else{
		       printf(" active Erase QSPI Sector parament success\r\n ");
			     printf(" warning : please close  #define CLEAN_SYS_PARAMENT\r\n ");
		      }
	#endif
	
	/* 读取参数标志位 确定是否存在配置参数 */
	if(QSPI_W25Qxx_ReadBuffer
		         (read_sys_parament_buffer, 
	              SYS_PARAMENT_ADDRESS, 
	                sizeof(read_sys_parament_buffer))
								    !=QSPI_W25Qxx_OK)
  {
		printf(" read QSPI system parament err\r\n ");
		return -1;
	}
	
//	for(uint8_t i=0;i<4;i++)	{
//	 printf(" read_sys_parament_buffer[%d]=%02x\r\n",i,read_sys_parament_buffer[i]);
//	 
//	}
	
	if(read_sys_parament_buffer[2]!=PARA_FLAG){ /* 不含有系统参数配置 */
		  printf(" not found system parament flag \r\n");
		
		 /* 擦除 最后一个扇区*/
		 if(QSPI_W25Qxx_SectorErase(SYS_PARAMENT_ADDRESS)!=QSPI_W25Qxx_OK){
			 printf(" Erase QSPI Sector parament err\r\n ");
		   return -1;
		 }
	 
	    //         写入数据       (结构体配置参数)    写入地址        数据大小(字节)
		 if(QSPI_W25Qxx_WriteBuffer(psys_parameter, SYS_PARAMENT_ADDRESS, sizeof(sys_parameter))!=QSPI_W25Qxx_OK){
			 printf(" Write QSPI Sector parament err\r\n ");
		   return -1;
		 }else{
		   printf(" will Write QSPI Sector parament success\r\n ");
		 }
		 
			 for(uint8_t i=0;i<4;i++)	{
		       printf(" sys_parament_buffer[%d]=%02x\r\n",i,psys_parameter[i]);
	    }
			
		}else{  /* 读取到系统配置参数 */
	
	
		/* QSPI中读取到的参数同步到结构体  其他函数直接访问结构体获取参数*/
					sys_parameter.jump_time=read_sys_parament_buffer[0];
				sys_parameter.which_app_running = read_sys_parament_buffer[1];  
			sys_parameter.sys_parameter_flag =read_sys_parament_buffer[2];  
		sys_parameter.debug_flag = read_sys_parament_buffer[3];
	
		/* 打印配置参数 */	
		printf("**************************SYS   PARAMENT******************************************\r\n");
			printf("sys_parameter.jump_time = 0x%02x\r\n",sys_parameter.jump_time);
				printf("sys_parameter.which_app_running = 0x%02x\r\n",sys_parameter.which_app_running);
					printf("sys_parameter.sys_parameter_flag = 0x%02x\r\n",sys_parameter.sys_parameter_flag);
						printf("sys_parameter.debug_flag = 0x%02x\r\n",sys_parameter.debug_flag);
							printf("******************************************************************************\r\n\r\n");                                     
		            return 0;
	     }
		
	return 0;
  
}
	


/* 选择直接跳转到APP （调用该函数之前尽量对数据合法性检查）
 * 参数可选1/2
 *
 * 失败返回 -1
 */
int app_exist_which_app(uint8_t para_flag)
{
  
	 if(QSPI_W25Qxx_MemoryMappedMode()!=QSPI_W25Qxx_OK) {
	  printf(" enable QSPI memory map mode err\r\n");
		 return -1;
	 }
	 
	  printf(" enable QSPI memory map mode success\r\n");
	  display_logo();
	 if(para_flag==1){
	    jump_app(QSPI_APP1_MM_BASE_ADDRESS);
	 }
	 else if(para_flag==2){
	   jump_app(QSPI_APP2_MM_BASE_ADDRESS);
	 }else{
	 
	 /* 填写参数错误 */
	 }
   return -1;
}






/* 短按功能 : 
 *
 * APP1与APP2之间切换
 *
 */
 int HandleShortPress(void)
{ 

//  擦除QSPI上的固件
//	if(QSPI_W25Qxx_BlockErase_32K(0)==QSPI_W25Qxx_OK){
//	
//	printf(" Erase QSPI Sector parament success\r\n ");
//	}
//	if(QSPI_W25Qxx_BlockErase_32K(0x400000)==QSPI_W25Qxx_OK){
//	
//	printf(" Erase QSPI Sector parament success\r\n ");
//	}
	
 
	BOOT_SYS_config* psys_parameter = &sys_parameter ;
	uint8_t read_buffer[4];
	
	
	/* 判断是否有两个APP存在 */
	int app_exist_state = check_qspi_app_exist();/* 上电检查APP是否存在 以及校验 */
	
	/* 只有一个APP退出 */
	if(app_exist_state!=3){
	  printf(" err Only one app exists\r\n");
	  return -1;
	}
	/* sys parament 读取到缓冲区 read_buffer */
  if(QSPI_W25Qxx_ReadBuffer
		                    (read_buffer, 
	                           SYS_PARAMENT_ADDRESS, 
	                                 sizeof(read_buffer) )
								                         !=QSPI_W25Qxx_OK)
      {
		   printf(" read QSPI parament err\r\n ");
		   return -1;
	    }else{
			 /* 擦除系统参数扇区 改写数据并写入 */
			    if(QSPI_W25Qxx_SectorErase(SYS_PARAMENT_ADDRESS)!=QSPI_W25Qxx_OK){
			    printf(" Erase QSPI Sector parament err\r\n ");
		      return -1;
		     }
			/* 改写参数 再写回 read_buffer[1]  app选项字节*/
				 
      if(read_buffer[1]==1){
		      read_buffer[1]=2;
		  }else if(read_buffer[1]==2){
		      read_buffer[1]=1;
		     }
			
			 if(QSPI_W25Qxx_WriteBuffer(read_buffer,SYS_PARAMENT_ADDRESS,sizeof(read_buffer))!=QSPI_W25Qxx_OK){
				  printf(" QSPI Write sys parament err\r\n ");
				 }
			 
			  /* 同步到结构体 */
				 psys_parameter->jump_time = read_buffer[0];
				 psys_parameter->which_app_running = read_buffer[1];
				 psys_parameter->sys_parameter_flag = read_buffer[2];
				 psys_parameter->debug_flag = read_buffer[3];
				 
				
				 app_exist_which_app(psys_parameter->which_app_running); /* 最开始检验过合法性，直接跳转 */
	 		
      }
	
	 
	     printf(" system which app parament err: %02x",psys_parameter->which_app_running);
		   for( ; ; ){
		              LEDR_TOGGLE();
		              delay_ms(300);
		             }

}



	

/* 长按功能 主动更新SD卡固件 */
int HandleLongPress(void)
{

  if(sys_parameter_config()!=0){ /* 读取/写入 系统参数 */
					 return -1; 
	        }
	
	uint8_t sys_which_app_update = psys_parameter->which_app_running;
	int sd_Firmware_check = Read_app_in_sd_card();
		
	/* 检查SD卡中所有固件 并检查QSPI中是否存在固件并更新 */
 	  if(sd_Firmware_check>=1&&sd_Firmware_check<=3){
		    printf("  Firmware found on SD card\r\n "); /* 检测到合法固件 */
			  printf(" long key : now will update firmware\r\n");
		}else{
        printf("  Firmware not found on SD card\r\n ");
		    return -1;
		} 
	


	
	switch(sys_which_app_update)
	{
	  case 1:
			    which_jump_to_app1_or_app2(sys_which_app_update); /* 跳转到app1执行 */
		      break;
		case 2:
			    which_jump_to_app1_or_app2(sys_which_app_update); /* 跳转到app2执行 */
			    break;
		default:
			    
			   for( ; ; ){
		   	            LEDR_TOGGLE();    /* 系统参数错误 */
			              printf("  sys_parameter err: %02x",psys_parameter->which_app_running);
		                delay_ms(300);
		               }
//	        break;

	}
	
	return -1;
}



//	__be32		ih_magic;	  /* Image Header Magic Number	*/
//	__be32		ih_hcrc;	  /* Image Header CRC Checksum	*/
//	__be32		ih_time;	  /* Image Creation Timestamp	*/
//	__be32		ih_size;	   /* Image Data Size		*/
//	__be32		ih_load;	   /* Data	 Load  Address		*/
//	__be32		ih_ep;		  /* Entry Point Address		*/
//	__be32		ih_dcrc;	  /* Image Data CRC Checksum	*/
//	uint8_t		ih_os;		  /* Operating System		*/
//	uint8_t		ih_arch;	  /* CPU architecture		*/
//	uint8_t		ih_type;	  /* Image Type			*/
//	uint8_t		ih_comp;	   /* Compression Type		*/
//	uint8_t		ih_name[IH_NMLEN];	/* Image Name		*/

///*大端字节序转小端字节序*/
//uint32_t my_ntohl(uint32_t netlong) {
//    return ((netlong & 0xFF000000) >> 24) |
//           ((netlong & 0x00FF0000) >> 8) |
//           ((netlong & 0x0000FF00) << 8) |
//           ((netlong & 0x000000FF) << 24);
//}



///*打印头部信息*/
//void printf_app_head(image_header_t *pimage_header_t , char *name){
//	
//    if (strcmp(name, APP1_FILE_NAME) == 0) {
//        printf("| app1 head information     \r\n");
//    } else if (strcmp(name, APP2_FILE_NAME) == 0) {
//        printf("| app2 head information     \r\n");
//    }

//	# if(0)   //用于调试
//   printf("ih_magic in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_magic));
//	 printf("ih_hcrc in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_hcrc));
//	 printf("ih_time in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_time));
//	 printf("ih_size in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_size));
//	 printf("ih_load in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_load));
//	 printf("ih_ep in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_ep));
//	 printf("ih_dcrc in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_dcrc));
//	
//	 printf("ih_os in hexadecimal: 0x%x\r\n",   pimage_header_t->ih_os);
//	 printf("ih_arch in hexadecimal: 0x%x\r\n", pimage_header_t->ih_arch);
//	 printf("ih_type in hexadecimal: 0x%x\r\n", pimage_header_t->ih_type);
//	 printf("ih_comp in hexadecimal: 0x%x\r\n", pimage_header_t->ih_comp);
//	 printf("| ih_name is: ");
//   for (int i = 0; i < IH_NMLEN; i++) 
//	 printf("%c", (unsigned char)pimage_header_t->ih_name[i]);  printf("\r\n");
//	#else
//	 printf("| ih_name   |");
//   for (int i = 0; i < IH_NMLEN; i++) 
//	 printf("%c", (unsigned char)pimage_header_t->ih_name[i]);  printf("\r\n");
//	
//	 printf("| ih_size   |  %u Byte    \r\n", my_ntohl(pimage_header_t->ih_size));
//	 printf("| ih_load   |  0x%x       \r\n", my_ntohl(pimage_header_t->ih_load));
//	 printf("| ih_ep     |  0x%x       \r\n", my_ntohl(pimage_header_t->ih_ep));
//	 printf("| ih_hcrc   |  0x%x       \r\n", my_ntohl(pimage_header_t->ih_hcrc)); //头部crc
//	 printf("| ih_dcrc   |  0x%x       \r\n", my_ntohl(pimage_header_t->ih_dcrc)); //app crc32
//	 printf("\r\n");
//	#endif
// 
//}
//	


//void check_app_head(image_header_t *app_image_header_t, char *app_name) {
//    int bytesRead = 0;

//    bytesRead = readFileContent(app_name, app_image_header_t, sizeof(image_header_t), 0);

//    if (bytesRead != sizeof(image_header_t)) {
//			  if (strcmp(app_name, APP1_FILE_NAME) == 0) 
//					{
//           printf("Error reading the APP file\r\n");
//          } 
//					else if (strcmp(app_name, APP2_FILE_NAME) == 0) 
//					{
//           printf("Error reading the APP file\r\n");
//          }
//           
//          } 
//		else {
//         printf_app_head(app_image_header_t, app_name);
//			
//    }
//}




 
/* 检查sd卡中app是否存在 
 * return 1   app1存在且合法
 * return 2   app2存在且合法
 * return 3   双App存在且合法
 * return 4   未找到APP
*/
//image_header_t app1_image_header_t;
//image_header_t app2_image_header_t;

int Read_app_in_sd_card(void)
{
	uint8_t app1_sp = AHEAD_SP;  
	uint8_t app1_pc = AHEAD_PC;
	uint8_t app2_sp = AHEAD_SP;  
	uint8_t app2_pc = AHEAD_PC;
	
	uint8_t app1_check_buffer[8];
	uint8_t app2_check_buffer[8];
	
	uint8_t app1_name_flag=0,app2_name_flag=0;    /* app文件名存在标志位    0:不存在  1:存在*/
	uint8_t app1_check_flag=0,app2_check_flag=0;  /* app文件校验合法标志位                  */
	
	int bytesRead=0; /* sd读取字节数*/
	int res;

	
	mount_sd(); /* 挂载失败停止*/

	 /* 搜索sd卡中app1文件名是否存在 */
	 res = f_stat(APP1_FILE_NAME, &fileInfo);
				if (res != FR_OK){
				      printf(" APP1 not found in SD card \r\n"); 
				}else{
				      app1_name_flag=1; 
				     }  
	 /* 搜索sd卡中app2文件名是否存在 */		
	 res = f_stat(APP2_FILE_NAME, &fileInfo);
				if (res != FR_OK){
				      printf(" APP2 not found in SD card \r\n");	
				}else{
              app2_name_flag=1;
				     }
	
				if(!(app1_name_flag+app2_name_flag)){
             return 4;  /* APP都不存在 */
				}					
				
			
				
				
	
	if(app1_name_flag) /* 读取到APP1文件名 检查app1数据合法性 */
	{
//	   check_app_head(&app1_image_header_t,APP1_FILE_NAME);
	      /* 读取app1前八个字节到缓冲区 */
//				bytesRead = readFileContent(APP1_FILE_NAME, app1_check_buffer, sizeof(app1_check_buffer), 0);
		bytesRead = readFileContent(APP1_FILE_NAME, app1_check_buffer, sizeof(app1_check_buffer), 64);
				if (bytesRead != sizeof(app1_check_buffer))/* 读取字节数不一致*/
				{
						printf("  Error reading the APP1 check file\r\n");
					  
				}else{
				           
				        if((app1_check_buffer[3]==app1_sp)&&(app1_check_buffer[7]==app1_pc)){ /* 校验app1合法性 */
		                app1_check_flag = 1; /* APP1 存在且合法 */
		            }else{
								      printf("  APP1 exists but is invalid\r\n");  /* APP1存在但不合法 */
								}
								
	           }			
	}
	
	if(app2_name_flag) /* 读取到APP2文件名 检查app数据合法性 */
	{
//		check_app_head(&app2_image_header_t,APP2_FILE_NAME);
		       /* 读取app2前八个字节到缓冲区 */
//				bytesRead = readFileContent(APP2_FILE_NAME, app2_check_buffer, sizeof(app2_check_buffer), 0);
		bytesRead = readFileContent(APP2_FILE_NAME, app2_check_buffer, sizeof(app2_check_buffer), 64);
				if (bytesRead != sizeof(app2_check_buffer))/* 读取字节数不一致*/
				{
						printf("  Error reading the APP2 check file\r\n");
					  
				}else{
				        /* 校验app2合法性 */
				        if((app2_check_buffer[3]==app2_sp)&&(app2_check_buffer[7]==app2_pc)){
		                app2_check_flag = 1; /*APP2 存在且合法*/
		            }else{
								      printf("  APP2 exists but is invalid\r\n"); 
								     }									
								
	           }			
	     
	} 
	
	
	 
	 if(app1_check_flag&&app2_check_flag) {
		 printf("  SD check APP1 and APP2 success\r\n" ); /* 两个app都存在且合法 */
		 return 3; 
	 }else if(app2_check_flag){
		 printf("  SD check  APP2 success\r\n");/* 只有app2合法 */
	   return 2;
	 }else if(app1_check_flag){
		 printf("  SD check  APP1 success\r\n"); /* 只有app1合法*/
	   return 1;
	 }else{
		 
	    /* 存在不合法APP */
	 }
	 
	 return 0; 
	 
}




void CheckButtonPress(void)
{
    static uint32_t pressTime = 0;        // 用于保存按钮被按下时的时间的变量
    static uint8_t buttonState = 0;       // 用于保存按钮状态的变量（0表示释放，1表示按下）
    static uint8_t longPressDetected = 0; // 用于跟踪是否检测到长按的变量

    if (READ_KEY() == GPIO_PIN_RESET)  // 按钮被按下
    {
        if (buttonState == 0)          // 按钮刚刚被按下
        {
            buttonState = 1;           // 更新按钮状态为按下
            pressTime = get_tick_ms(); // 记录按钮被按下时的时间
            longPressDetected = 0;     // 重置长按检测标志
        }
    }
    else  // 按钮被释放
    {
        if (buttonState == 1)  // 按钮刚刚被释放
        {
            buttonState = 0;  // 更新按钮状态为释放
            uint32_t releaseTime = get_tick_ms();  // 获取按钮被释放时的时间
            uint32_t duration = releaseTime - pressTime;  // 计算按钮被按下的持续时间

            if (duration >= LONG_PRESS_TIME)
            {
                HandleLongPress();// 检测到长按
                longPressDetected = 1;  // 设置标志以指示已检测到长按
            }
            else if (duration >= SHORT_PRESS_TIME && !longPressDetected)
            {
                HandleShortPress();// 检测到短按
            }
        }
    }

    delay_ms(1);  // 去抖延时
}



/* 根据参数选择哪一个app启动  
 * 参数：   1/2
 */
int which_jump_to_app1_or_app2(uint8_t para_flag)
{
	
	  printf("  get system parament = 0x%0x\r\n",para_flag);
	
   if(para_flag==1){  /* 跳转到app1 */
		 
	  /* 这样看起来有些 冗余  我的本意是未读取到配置参数就不初始化这些外设 */
		 
//		     bsp_sd_init(); /* 挂载SD ，失败会停止执行 */
		     SDRAM_InitSequence(); /* 初始化sdram */  
		     
					if(read_sd_data_to_sdram(1)==0){ /* 比较sd与sdram数据一致 */
						 if(write_sdram_data_to_qspi_flash(1)==0){ /* 将sdram中数据写入qspi 开启内存映射模式 比较与sdram数据一致 */
							     QSPI_W25Qxx_MemoryMappedMode(); 
					         verify_qspi_data(1);
							     printf(" Update APP1 success!!!\r\n");
								   display_logo();
	                 jump_app(QSPI_APP1_MM_BASE_ADDRESS);
						    }
					    /*SDRAM 与 QSPI数据不一致 */
							printf("APP1 SDRAM compare QSPI data different err\r\n");
						}
		        /*SD 与 SDRAM数据不一致 */
		      printf("APP1 SD compare SDRAM data different err\r\n");
						
						
	   }else if(para_flag==2){ /* 跳转到app2区域 */
					
//					  bsp_sd_init(); /* 挂载SD ，失败会停止执行 */
						SDRAM_InitSequence(); /* 初始化sdram */
		 
							if(read_sd_data_to_sdram(2)==0){ /* 比较sd与sdram数据一致 */
								 if(write_sdram_data_to_qspi_flash(2)==0){ /* 将sdram中数据写入qspi 开启内存映射模式 比较与sdram数据一致 */
									  QSPI_W25Qxx_MemoryMappedMode(); 
					          verify_qspi_data(1);
									  printf(" Update APP2 success!!!\r\n");
				            display_logo();
	                  jump_app(QSPI_APP2_MM_BASE_ADDRESS);
						    }
				      /*SDRAM 与 QSPI数据不一致 */
						 printf(" APP2 SDRAM compare QSPI data different err\r\n");
						}
		        /*SD 与 SDRAM数据不一致 */
            printf(" APP2 SD compare SDRAM data different err\r\n");

		 
	 }else{/* 未读取到配置参数 */
		 
			
	    /* 警告 未读取到参数，检查QSPI */
		 printf(" system parament err or qspi not init : jump to app stop\r\n ");
	   }
    return -1;
}



/* 根据Read_app_in_sd_card返回值 将存在的APP写入QSPI
 *
 */
void Check_the_app_in_the_sd_card(void){
   
	       int read_sd=0;
	    	 read_sd = Read_app_in_sd_card(); /*检查sd卡中是否存在APP，不存在提示插入sd卡 存在则识别有几个APP*/
		   
		     if(read_sd>=1&&read_sd<=3){
		            SDRAM_InitSequence();/* 初始化SDRAM */
					      QSPI_W25Qxx_Init(); /* 初始化QSPI*/
		           } 
		 
		     if(read_sd==1){  
				  /* SD中存在APP1数据 */
					printf(" Update APP1 from sd card\r\n");
          read_sd_data_to_sdram(1);
					write_sdram_data_to_qspi_flash(1);
					QSPI_W25Qxx_MemoryMappedMode();
          verify_qspi_data(2);					 
					jump_app(QSPI_APP1_MM_BASE_ADDRESS);
					
				 }else if(read_sd==2){/* SD中存在APP2数据 */
					 printf(" Update APP2 from sd card\r\n");
					 read_sd_data_to_sdram(2);
					 write_sdram_data_to_qspi_flash(2);
					 QSPI_W25Qxx_MemoryMappedMode(); 
					 verify_qspi_data(2);
					 jump_app(QSPI_APP2_MM_BASE_ADDRESS);
					 
				 }else if(read_sd==3){/* SD中存在APP1 与 APP2数据 */
					 printf(" Update APP1 and APP2 from SD card to QSPI\r\n");
					 read_sd_data_to_sdram(1);
					 read_sd_data_to_sdram(2);
					 write_sdram_data_to_qspi_flash(1);
					 write_sdram_data_to_qspi_flash(2);
					 QSPI_W25Qxx_MemoryMappedMode(); 
					 verify_qspi_data(1); /* 数据校验 */ 
					 verify_qspi_data(2);
					 jump_app(QSPI_APP2_MM_BASE_ADDRESS);/* 第一次加载默认APP1 */
					 
//					 0x90000000UL
				 }else{
				        /* 未检测到SD卡中存在APP */
					      for( ; ; )
					      {
					       printf(" APP not found  please add firmware\r\n");
						     delay_ms(500);
                }
				 
				      }


}





	

/* 获取系统参数 which app 二选一启动 */
int Dual_app_exist_which_app(void)
{
	 BOOT_SYS_config * psys_parameter = &sys_parameter;	
	
	 if(sys_parameter_config()!=0){
					 return -1; /* 读取/写入 系统参数失败*/
	        }
            if((psys_parameter->which_app_running==1)||(psys_parameter->which_app_running==2))
						{
						 app_exist_which_app(psys_parameter->which_app_running);/* 读取配置参数选择启动app */
						}else{ /* which_app_running参数错误*/
                  for( ; ; ) {   
															LEDR_TOGGLE();
															printf(" sys which app parament err %d\r\n",psys_parameter->which_app_running);
															delay_ms(300);
			                       }
                 } 
    return -1;
}


	
 /*  利用SD卡将数据刷入到QSPI APP1 or APP2 地址运行  */
 int read_app_and_jump_running(void)
 {
	 
	 	if(QSPI_W25Qxx_Init()!=QSPI_W25Qxx_OK){
	     printf(" W25Q64 init err \r\n");
		   return -1;
	    }
		
		
		if(sys_parameter_config()!=0){ /* 读取/写入 系统参数 */
			 printf(" W25Q64 init err \r\n");
			 return -1; 
	    }
		
	 printf(" current sys parament = %d\r\n",psys_parameter->which_app_running);
	
		
		
	/* 上电倒计时内未触发按键则正常启动   短按：切换APP  长按(弹开有效)：强制更新APP(需插入SD卡) */	
		
	 key_init();
	
//	 printf(" If no operation is performed within %ds, the system will jump\r\n",JUMP_TIME);
//	 for(int i=JUMP_TIME*1000 ;i>=0 ;i--) 
//	 {
//		  if(i%1000==0)   printf(" %ds later jump  \r\n", i/1000);  /* 打印倒计时 */ 
//     
//      CheckButtonPress();/* 检测按键是否按下  函数中带有 1ms 延时 */
//	 }
		
		
	 
//	 int app_exist_state = check_qspi_app_exist();/* 上电检查APP是否存在 以及校验 */
   int app_exist_state =4;

	 switch(app_exist_state){
		case 1: 
						app_exist_which_app(1);  /* QSPI中只有APP1存在 直接启动 */
						break;
		
		case 2: 
			      app_exist_which_app(2);  /* QSPI中只有APP2存在 直接启动 */
					  break;
		
		case 3:
			      Dual_app_exist_which_app(); /* QSPI中双APP存在 根据参数选择启动 参数可通过按键调整 具有记忆功能 */
			      break;    			
		
		default:
			      Check_the_app_in_the_sd_card(); /* QSPI中未检测到APP   检测SD卡中是否存在APP  */
			      break;
   }
		
     for( ; ; ){
		   
		  /* 运行到这里错误 */
		 }
 
}
 



/*
 配置参数：
  
 CRC32                 4字节
 是否升级标志位        1字节
 倒计时                1字节
 启动时是否开启CRC校验
 启动时候是否开启备份数据校验
 是否主动升级标志位
  
 
 

 

*/

/*

  
	 










*/









/* 软件crc32计算公式 */

#define CRC32_POLYNOMIAL 0xEDB88320u

uint32_t calculate_crc32(const void *data, size_t size) {
    const uint8_t *byteData = (const uint8_t *)data;
    uint32_t crc = 0xFFFFFFFFu;

    for (size_t i = 0; i < size; ++i) {
        crc ^= byteData[i];
        for (int j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^ ((crc & 1) ? CRC32_POLYNOMIAL : 0);
        }
    }

    return ~crc;
}







