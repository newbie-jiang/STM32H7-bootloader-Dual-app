#include "api_sdram.h"
#include "bsp_sdram.h"



uint32_t api_sdram_init(void)
{
	 return bsp_sdram_init();
	
}

uint32_t api_sdram_config(void)
{
	 SDRAM_InitSequence();
}


 sdram_driver ssdram_driver = {
  .sdram_init = api_sdram_init,
	.sdram_config = api_sdram_config,
 };
 
#define SDRAM_BANK_ADDRESS     ((uint32_t)0xC0000000)

uint32_t tets_sdram_driver(sdram_driver * psdram_driver)
{
	uint8_t test_buffer_arr[1024] ={0};
	psdram_driver->sdram_init();
	psdram_driver->sdram_config();
	

	volatile uint32_t i=0;  	  
	volatile uint32_t temp=0;	   
	volatile uint32_t read_val=0;	//用于读取SDRAM中的数据
	volatile uint8_t success_flag=1; //标志位，用于检查数据是否一致
		

		//每隔16K字节,写入一个数据,总共写入2048个数据,刚好是32M字节
		for(i=0;i<32*1024*1024;i+=16*1024)
		{
			*(__IO uint32_t*)(SDRAM_BANK_ADDRESS+i)=temp; 
			temp++;
		}

		//重置temp值
		temp=0;

		//依次读出之前写入的数据,进行校验		  
		for(i=0;i<32*1024*1024;i+=16*1024) 
		{	
				read_val=*(__IO uint32_t*)(SDRAM_BANK_ADDRESS+i);    //SDRAM_BANK_ADDR   BACK0的基地址
			if(read_val != temp)
			{
				success_flag=0; //如果读取的数据与写入的数据不匹配，设置标志位为0
				break;
			}
			temp++;
		}	
		

			if(success_flag)
			{
				printf("  checkout sdram : success!!!\r\n");
				return 0;
			}
			else
			{
				printf("  checkout sdram : err!!!\r\n");
				return -1;
			}		
	

}

