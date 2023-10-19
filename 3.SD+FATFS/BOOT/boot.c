#include "boot.h"

#include "bsp_qspi.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_sd.h" 




/* QSPI 被映射的基地址*/
#define QSPI_BASE_ADDRESS  0X90000000UL
 
 typedef  void (*pFunction)(void);
 pFunction JumpToApplication;
 
 
 
 

 void jump_app(void);
 int  qspi_init_and_enable_MemoryMapMode(void);
 void display_logo(void);



void  jump_to_qspi_flash(void)
{
	
   sd_application();
	
	
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


/* 确保一个干净的bootloader 我们需要解初始化用到的所有外设以及中断  只保留QSPI，在APP中就不需要动QSPI了 */
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
	 
	 /*解初始化LED*/ 
	 HAL_GPIO_DeInit(LED_R_GPIO_Port,LED_R_Pin);
	 HAL_GPIO_DeInit(LED_B_GPIO_Port,LED_R_Pin);
	 
	 /*解初始化usart */
   HAL_UART_MspDeInit(&huart4);
	 
	 /*设置所有时钟默认状态恢复HSI*/
	 HAL_RCC_DeInit();
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

