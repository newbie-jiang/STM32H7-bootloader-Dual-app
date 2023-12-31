#include "api_qspi.h"
#include "bsp_qspi.h"

#include "stdio.h"
#include "string.h"

uint32_t api_qspi_falsh_init(void)
{
  return QSPI_W25Qxx_Init(); 
}

uint32_t api_qspi_falsh_reset(void)
{
	
	return QSPI_W25Qxx_Reset();

}

uint32_t api_qspi_falsh_read_id(void)
{
	return QSPI_W25Qxx_ReadID();
}


uint32_t api_qspi_falsh_memorymap(void)
{
  return QSPI_W25Qxx_MemoryMappedMode();
}


uint32_t api_qspi_falsh_SectorErase_4k(uint32_t SectorAddress)
{
 return QSPI_W25Qxx_SectorErase(SectorAddress);
}



uint32_t api_qspi_falsh_BlockErase_32k(uint32_t SectorAddress)
{
	return QSPI_W25Qxx_BlockErase_32K(SectorAddress);

}

uint32_t api_qspi_falsh_BlockErase_64k(uint32_t SectorAddress)
{
	return QSPI_W25Qxx_BlockErase_64K(SectorAddress);
}

uint32_t api_qspi_falsh_ChipErase()
{
  return QSPI_W25Qxx_ChipErase();

}

uint32_t api_qspi_falsh_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	return QSPI_W25Qxx_WritePage( pBuffer,  WriteAddr,  NumByteToWrite);
}

uint32_t api_qspi_falsh_WriteBuffer(uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
	return QSPI_W25Qxx_WriteBuffer( pData,  WriteAddr,  Size);

}

uint32_t api_qspi_falsh_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{

 return QSPI_W25Qxx_ReadBuffer(pBuffer, ReadAddr,  NumByteToRead);

}



api_qspi_flash_driver  sapi_qspi_flash_driver = {
	
   .qspi_falsh_init           = api_qspi_falsh_init,
   .qspi_falsh_reset          = api_qspi_falsh_reset,
	 .qspi_falsh_read_id        = api_qspi_falsh_read_id,
	 .qspi_falsh_memorymap      = api_qspi_falsh_memorymap,
	 .qspi_falsh_SectorErase_4k = api_qspi_falsh_SectorErase_4k,
	 .qspi_falsh_BlockErase_32k = api_qspi_falsh_BlockErase_32k,
	 .qspi_falsh_BlockErase_64k = api_qspi_falsh_BlockErase_64k,
	 .qspi_falsh_ChipErase      = api_qspi_falsh_ChipErase,
	 .qspi_falsh_WritePage      = api_qspi_falsh_WritePage,
	 .qspi_falsh_WriteBuffer    = api_qspi_falsh_WriteBuffer,
	 .qspi_falsh_ReadBuffer     = api_qspi_falsh_ReadBuffer,
};


uint32_t test_qspi_flash_driver(api_qspi_flash_driver * papi_qspi_flash_driver)
{
   papi_qspi_flash_driver->qspi_falsh_init();
	
	 uint32_t flash_id = papi_qspi_flash_driver->qspi_falsh_read_id();
	
	 printf(" qspi falsh id is %x\r\n",flash_id);
	
	 papi_qspi_flash_driver->qspi_falsh_SectorErase_4k(0x00);//Erases the address as a multiple of page
	 
	 uint8_t test_arr[100] ={0};
	 uint8_t read_arr[100] ={0};
	 
	 for(int i=0; i<100; i++){
	    test_arr[i] = i;
	 }
	
	 papi_qspi_flash_driver->qspi_falsh_WriteBuffer(test_arr,0x00,sizeof(test_arr));
	 papi_qspi_flash_driver->qspi_falsh_ReadBuffer(read_arr,0x00,sizeof(test_arr));
	 if(0 == memcmp(test_arr,read_arr,sizeof(test_arr))){
	   printf(" test qspi flash read wright success\r\n");
	 }else{
	   printf(" test qspi flash read wright err\r\n");
		 return -1;
	 }
	 
	 
	 papi_qspi_flash_driver->qspi_falsh_memorymap(); //start memorymap
	 
	 volatile uint8_t * buffer = ( volatile uint8_t *)0x90000000;
   for(int i=0; i<100; i++){
		   if(test_arr[i] != * buffer){
			    printf(" test qspi flash memorymap  err\r\n");
		      return -1;
			   }
	       buffer++;
		 }
	  
	      printf(" test qspi flash memorymap  success\r\n");
}