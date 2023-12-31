#ifndef _API_QSPI_H_
#define _API_QSPI_H_

#include "stdint.h"
#include "stdbool.h"


typedef struct api_qspi_flash_driver{
  uint32_t (*qspi_falsh_init)(void);
  uint32_t (*qspi_falsh_reset)(void);
  uint32_t (*qspi_falsh_read_id)(void);
	uint32_t (*qspi_falsh_memorymap)(void);
	uint32_t (*qspi_falsh_SectorErase_4k)(uint32_t address);   
	uint32_t (*qspi_falsh_BlockErase_32k)(uint32_t address);
	uint32_t (*qspi_falsh_BlockErase_64k)(uint32_t address);
	uint32_t (*qspi_falsh_ChipErase)(void);
	uint32_t (*qspi_falsh_WritePage)(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
	uint32_t (*qspi_falsh_WriteBuffer)(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);
	uint32_t (*qspi_falsh_ReadBuffer)(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
}api_qspi_flash_driver;

extern api_qspi_flash_driver sapi_qspi_flash_driver;

uint32_t test_qspi_flash_driver(api_qspi_flash_driver * papi_qspi_flash_driver);





#endif /* _API_QSPI_H_ */
