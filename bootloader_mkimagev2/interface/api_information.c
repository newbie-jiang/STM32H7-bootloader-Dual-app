#include "api_information.h"
#include "bsp_information.h"

#include "stdio.h"


/*********************************************************************/

static uint32_t api_get_SysClockFreq(void){
	
    return get_stm32h7_SysClockFreq();
}

static uint32_t api_get_HCLKFreq(void){

    return get_stm32h7_HCLKFreq();
}

static uint32_t api_get_PCLK1Freq(void){

    return get_stm32h7_PCLK1Freq();
}

static uint32_t api_get_PCLK2Freq(void){

   return get_stm32h7_PCLK2Freq();
}


 api_clock_info sapi_clock_info = {
   .get_SysClockFreq =  api_get_SysClockFreq ,
	 .get_HCLKFreq     =  api_get_HCLKFreq,
	 .get_PCLK1Freq    =  api_get_PCLK1Freq,
	 .get_PCLK2Freq    =  api_get_PCLK2Freq,
};
 

void test_clock_info(api_clock_info * pclock_info){
	
	 uint32_t SysClockFreq,HCLKFreq,PCLK1Freq,PCLK2Freq;
	
	 SysClockFreq = pclock_info->get_SysClockFreq();
   HCLKFreq = pclock_info->get_HCLKFreq();
	 PCLK1Freq = pclock_info->get_PCLK1Freq();
	 PCLK2Freq = pclock_info->get_PCLK2Freq();
	
	 printf("SysClockFreq = %uHZ\r\n",SysClockFreq);
   printf("HCLKFreq     = %uHZ\r\n",HCLKFreq);
   printf("PCLK1Freq    = %uHZ\r\n",PCLK1Freq);
   printf("PCLK2Freq    = %uHZ\r\n",PCLK2Freq);
}

/*********************************************************************/



/*********************************************************************/
uint32_t  api_get_chipid(chip_id_which echip_id_which){
	 
	 /* stm32_h7_id */
	  
    uint32_t chip_id[3];	
	  if(echip_id_which==0){
		   chip_id[0] = * ( __IO uint32_t * ) ( 0x1FF1E800 ); //高32位地址
		   }
		else if(echip_id_which==1){
		   chip_id[1] = * ( __IO uint32_t * ) ( 0x1FF1E800+0X4 ); //中32位地址
		   }
		else if(echip_id_which==2){
		   chip_id[2] = * ( __IO uint32_t * ) ( 0x1FF1E800+0X8 ); //低32位地址
		   }
		else{
		   return chip_id[echip_id_which];
		}
		
			
		   return chip_id[echip_id_which];

 }
 


api_chip_id_info sapi_chip_id_info ={

	 .get_chipid = api_get_chipid,

};


void test_chip_id_info(){
	
	api_chip_id_info *papi_chip_id_info = &sapi_chip_id_info;
	
	uint32_t chip_id[3];
	
	chip_id[0] = papi_chip_id_info->get_chipid(id_0);
	chip_id[1] = papi_chip_id_info->get_chipid(id_1);
  chip_id[2] = papi_chip_id_info->get_chipid(id_2);
	printf ( "chip id is 0x %x-%x-%x \r\n", chip_id[0], chip_id[1], chip_id[2] );
}

/*********************************************************************/


/*********************************************************************/
uint32_t api_get_flash_size(void){
	
     uint32_t stm32_Flash_Size;
	
     stm32_Flash_Size = * ( uint16_t * ) ( 0x1FF1E880 ); //闪存容量寄存器
	
	   return stm32_Flash_Size;
 
}


 api_get_falsh_size sapi_get_falsh_size ={
	 
  .get_falsh_size = api_get_flash_size,
 
 };

 
 
 void test_api_get_falsh_size(void){
	 uint32_t Flash_Size;
	 
   api_get_falsh_size *papi_get_falsh_size = &sapi_get_falsh_size;
	 
   Flash_Size = papi_get_falsh_size->get_falsh_size();
	 
	 printf(" falsh size is %d KB\r\n",Flash_Size);
 
 }
 /*********************************************************************/