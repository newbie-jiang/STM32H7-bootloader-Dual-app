#include "boot.h"

#include "bsp_qspi.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_sd.h" 
#include "bsp_sdram.h" 



/* QSPI 被映射的基地址*/
#define     QSPI_BASE_ADDRESS  0x90000000UL

/* SDRAM 被映射的基地址*/
#define     SDRAM_TARGET_ADDR  0xC1000000UL

/* SD卡与SDRAM数据 对比缓冲区*/
#define     BLOCK_SIZE         1024   // 1KB

/* 读取文件名 */
#define     APP_FILE_NAME      "app.bin"




 typedef  void (*pFunction)(void);
 pFunction JumpToApplication;
  

 void jump_app(void);
 int  qspi_init_and_enable_MemoryMapMode(void);
 void display_logo(void);
 void read_sd_data_to_QSPI(void);


void  jump_to_qspi_flash(void)
{
	
	read_sd_data_to_QSPI();
	
	
//	 bsp_sdram_init();
	
//   sd_application();
	
	
//	 /* 开启内存映射 success*/
//	 if(qspi_init_and_enable_MemoryMapMode() == 0)
//	 {
//	  display_logo(); 
//		
//	  jump_app();
//	
//	 }
//	 else  /* 检测到QSPI未正常初始化|未开启内存映射模式 */
//	 {
//		 while(1)
//		 {
//		  LEDR_TOGGLE();
//			delay_ms(500);
//		 }
//	 }
}




/* return 0: success     return -1: err */
int qspi_init_and_enable_MemoryMapMode(void)
{
   volatile uint8_t id[2];
    
    if(QSPI_W25Qxx_Init()==QSPI_W25Qxx_OK){   /* qspi init */
			  printf(" QSPI Init success \r\n");
			
			  if(QSPI_W25Qxx_MemoryMappedMode()==QSPI_W25Qxx_OK){  /* start MemoryMappedMode */
				printf(" QSPI Start Memory Map success \r\n\r\n\r\n");	
				return 0;	
				}else{
				
				  printf(" QSPI Start Memory Map err \r\n ");
				  return -1;
				}					
	  }
		else{
			 printf(" QSPI Init err \r\n ");
			 return -1;
		 }
}




/* 以防万一 其他外设把qspi时钟关了，在最后开启qspi时钟 */
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
	 HAL_SDRAM_MspDeInit(&hsdram1);
		
	 
	 /*解初始化LED*/ 
	 HAL_GPIO_DeInit(LED_R_GPIO_Port,LED_R_Pin);
	 HAL_GPIO_DeInit(LED_B_GPIO_Port,LED_R_Pin);
		
	 /*解初始化 SD_CAP*/
	 HAL_GPIO_DeInit(SD_CAP_GPIO_Port,SD_CAP_Pin);
		
		
	 /*解初始化usart */
   HAL_UART_MspDeInit(&huart4);
	 
	
	/*设置所有时钟默认状态恢复HSI*/
	 HAL_RCC_DeInit();
	 
	 /* 为防止qspi时钟 被初始化 */
	 enable_qspi_clk();
}





 void jump_app(void)
 {
	/* 解初始化所有外设与中断 */
	DeInit_all_bsp_and_IRQ();
	 
	/* 确保CPU不响应任何中断，无论它们是否被使能。   注：在app中需要手动开启中断 __enable_irq(); */
	__disable_irq();
	 

	JumpToApplication = (pFunction) (*(__IO uint32_t*) (QSPI_BASE_ADDRESS + 4));	// 设置起始地址
	__set_MSP(*(__IO uint32_t*) QSPI_BASE_ADDRESS);	// 设置主堆栈指针

   
	JumpToApplication();			// 执行跳转
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




 FILINFO fileInfo;

/*将SD数据读取至sdram
  将SD数据读取至1k缓存,与sdram存放区域的每个字节进行校验  循环读取逐字校验
*/


int read_sd_data_to_sdram(void)
{
    
    FRESULT res;
    uint32_t bytesRead;
    uint8_t* sdram_ptr = (uint8_t*)SDRAM_TARGET_ADDR;  // 指向SDRAM的指针
    uint8_t temp_buffer[BLOCK_SIZE];  // 临时缓冲区

    // 获取文件信息
    res = f_stat(APP_FILE_NAME, &fileInfo);
    if (res != FR_OK)
    {
        printf("  Error getting file info.\r\n"); /*文件不存在*/
        return -1;
    }
    else
    {
        printf("  The size of the file  %s is %ld bytes.\r\n", APP_FILE_NAME,fileInfo.fsize); /*获取文件大小*/
    }

    // 读取整个文件到SDRAM
    bytesRead = readFileContent(APP_FILE_NAME, sdram_ptr, fileInfo.fsize+1, 0);
		
    if (bytesRead != fileInfo.fsize)
    {
        printf("  Error reading the entire file.\r\n");
        return -1;
    }

    // 以1KB的块大小进行校验
    for (uint32_t i = 0; i < fileInfo.fsize; i += BLOCK_SIZE)
    {
        // 从文件中读取1KB的块到临时缓冲区
        bytesRead = readFileContent(APP_FILE_NAME, temp_buffer, BLOCK_SIZE, i);
        if (bytesRead != BLOCK_SIZE && i + bytesRead != fileInfo.fsize)
        {
            printf("  Error reading the file block to temp buffer.\r\n");
            return -1;
        }

        // 比较SDRAM中的块和临时缓冲区中的数据
        if (memcmp(sdram_ptr + i, temp_buffer, bytesRead) != 0)
        {
            printf("  Data mismatch detected at block starting from byte %d.\r\n", i);
            return -1;
        }
    }

    printf("  Data in SDRAM check the file content success.\r\n");
		return 0;
		
}



  #define BUFFER_SIZE 1024
  #define W25Q64JV_SECTOR_SIZE   0x1000

  /* 将sdram中数据写入qspi flash 并校验 */
  int write_sdram_data_to_qspi_flash(void)
  {

		
    uint8_t* sdram_data_ptr = (uint8_t*)SDRAM_TARGET_ADDR;
		uint8_t* qspi_data_ptr = (uint8_t*)QSPI_BASE_ADDRESS;
		
    uint32_t remaining_bytes = fileInfo.fsize;
    
	//	uint8_t read_buffer[BUFFER_SIZE];
  //	uint32_t bytes_to_compare;
  //  uint32_t offset = 0;
		
		
		
    QSPI_W25Qxx_Init();
		
		/* 判断固件的数据大小 确定需要擦除的地址 0扇区开始 */
		uint32_t start_sector = 0;  // 假设数据始终从扇区0开始
    uint32_t end_sector = (fileInfo.fsize + W25Q64JV_SECTOR_SIZE - 1) / W25Q64JV_SECTOR_SIZE - 1;  // 计算结束的扇区

    uint32_t sectors_need_Erase_num = end_sector - start_sector + 1;  // 计算需要擦除的扇区数量

    printf("  fileInfo.fsize=%ld\r\n", fileInfo.fsize);
    printf("  sectors_need_Erase_num = %04d\r\n", sectors_need_Erase_num);

	
		/* 擦除使用到的扇区 擦除大小必须是4k整数倍 */
   for (uint32_t i = 0; i < sectors_need_Erase_num; i++) {
		 
		   if(QSPI_W25Qxx_SectorErase((i * W25Q64JV_SECTOR_SIZE))==QSPI_W25Qxx_OK) { // 使用i来确定要擦除的扇区地址
   
		   /*擦除成功*/
		    printf("  qspi erase %d sectors success!!!\r\n",i);
		}
		 else{
		  /*擦除失败*/
			 printf("  qspi erase err!!!\r\n");
			 return -1;
			 
		 }
		 
   }
	 
	
		
		/* 写入数据 */
		if(QSPI_W25Qxx_WriteBuffer(sdram_data_ptr, 0x0, fileInfo.fsize)!=QSPI_W25Qxx_OK)
		{
		 return -1;
		}
	
    
//    /* 非内存映射模式下的数据校验 */
//		while (remaining_bytes > 0) {
//    // 计算这一轮要比较的字节数
//    bytes_to_compare = (remaining_bytes > BUFFER_SIZE) ? BUFFER_SIZE : remaining_bytes;

//    // 从QSPI Flash中读取数据
//    QSPI_W25Q64JV_Read(read_buffer, offset, bytes_to_compare);

//    // 逐字节比较SDRAM和缓冲区中的数据
//    for (uint32_t i = 0; i < bytes_to_compare; i++) {
//        if (read_buffer[i] != sdram_data_ptr[offset + i]) {
//					 /*打印校验错误信息*/
//            printf("Mismatch at offset 0x%X: QSPI=0x%02X, SDRAM=0x%02X\r\n", 
//                   offset + i, read_buffer[i], sdram_data_ptr[offset + i]);
//            return -1;  // 校验失败
//        }
//    }

//    // 更新剩余字节数和偏移量
//    remaining_bytes -= bytes_to_compare;
//    offset += bytes_to_compare;
//   } 
//		/* 可以执行到这里则校验成功 */
//		printf("  qspi not memorymap checkout success!!!\r\n");
		

    /* 开启内存映射模式再进行校验 */
		QSPI_W25Qxx_MemoryMappedMode();
		
		
		/* qspi内存映射模式下与sdram的数据校验 */
		if(memcmp(sdram_data_ptr, qspi_data_ptr, remaining_bytes) == 0) {
			
		printf("  qspi  memorymap data check success!!!\r\n");
			
	 }
		else{
			printf("  qspi  memorymap data check err!!!\r\n");
		 return -1;
		}
		
		return 0;
	
}





  /*  利用SD卡将数据刷入到QSPI 基地址 */
 void read_sd_data_to_QSPI(void)
 {
	   bsp_sd_init(); /* 挂载SD ，失败会停止执行 */
	 
	   SDRAM_InitSequence();/*初始化sdram*/
	 
     if(read_sd_data_to_sdram()==0) /* 比较sd与sdram数据一致 */
		 {
		  write_sdram_data_to_qspi_flash(); /* 将sdram中数据写入qspi 开启内存映射模式 比较与sdram数据一致 */
		 }
   
 }

