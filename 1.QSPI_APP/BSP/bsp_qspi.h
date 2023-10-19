#ifndef W25Q64JV_H
#define W25Q64JV_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


#define QSPI_BASE_ADDR   0X90000000

#define W25Q64JV_FLASH_SIZE (8 * 1024 * 1024)
#define W25Q64JV_BLOCK_SIZE (0x10000)
#define W25Q64JV_SECTOR_SIZE (0x1000)   //擦除大小
#define W25Q64JV_PAGE_SIZE (0x100)

#define W25Q64JV_WRITE_ENABLE (0x06)
/*
 * The Quad Enable (QE) bit is set to 1 by default in the factory, therefore the device supports Standard/Dual
SPI as well as Quad SPI after power on. This bit cannot be reset to 0.
 */
#define W25Q64JV_INPUT_FAST_READ (0xeb)
#define W25Q64JV_PAGE_PROGRAM (0x02)
#define W25Q64JV_STATUS_REG1 (0x05)
#define W25Q64JV_ENABLE_RESET (0x66)
#define W25Q64JV_RESET_DEVICE (0x99)
#define W25Q64JV_DEVICE_ID (0x90)
#define W25Q64JV_ID_NUMBER (0x4b)
#define W25Q64JV_ERASE_SECTOR (0x20)

#define QSPI_OK 0
#define QSPI_ERROR -1


int  QSPI_W25Q64JV_Reset(void);
int  QSPI_W25Q64JV_DeviceID(uint8_t id[2]);
int  QSPI_W25Q64JV_IDNumber(uint8_t id[8]);

/*
 * 擦除特定地址
 * SectorAddress必须是W25Q64JV_SECTOR_SIZE整数倍
 */
int QSPI_W25Q64JV_EraseSector(uint32_t SectorAddress);

/*
 * 任意地址读取数据
 */
int QSPI_W25Q64JV_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);

/*
 * 在页范围内写数据,长度不能越界
 * 注意写之前要确保flash里面对应位置的数据全为0xFF
 */
int QSPI_W25Q64JV_PageProgram(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
/*
 * 必须确保写的地址范围内的数据全部为0xFF
 * 具有自动换页的功能
 */
int QSPI_W25Q64JV_Write_NoCheck(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
/*
 * 在地址空间内任意地址写任意长度(不超过地址空间)的数据
 */
int QSPI_W25Q64JV_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
int QSPI_W25Q64JV_EnableMemoryMappedMode(void);


 void bsp_qspi_init(void);

#ifdef __cplusplus
}
#endif
#endif
