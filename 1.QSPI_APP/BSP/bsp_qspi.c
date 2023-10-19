#include "bsp_qspi.h"
#include "main.h"
#include <string.h>
#include <stdio.h>

extern QSPI_HandleTypeDef hqspi;
/*
 * 写操作时先读取内部数据
 */
static uint8_t w25q64jv_buf[W25Q64JV_SECTOR_SIZE];

static int QSPI_W25Q64JV_AutoPollingMemReady(uint32_t timeout) {

    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_STATUS_REG1,
        .DataMode = QSPI_DATA_1_LINE,
    };

    QSPI_AutoPollingTypeDef conf = {
        .Match = 0x00,
        .Mask = 0x01,
        .MatchMode = QSPI_MATCH_MODE_AND,
        .StatusBytesSize = 1,
        .Interval = 0x10,
        .AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE,
    };

    if (HAL_QSPI_AutoPolling(&hqspi, &cmd, &conf, timeout) != HAL_OK) {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

static int QSPI_W25Q64JV_WriteEnable() {

    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_WRITE_ENABLE,
    };

    if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q64JV_STATUS_REG1;

    cmd.DataMode = QSPI_DATA_1_LINE;
    cmd.DummyCycles = 0;
    cmd.NbData = 0;

    QSPI_AutoPollingTypeDef conf = {
        .Match = 0x02,
        .Mask = 0x02,
        .MatchMode = QSPI_MATCH_MODE_AND,
        .StatusBytesSize = 1,
        .Interval = 0x10,
        .AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE,
    };

    if (HAL_QSPI_AutoPolling(&hqspi, &cmd, &conf, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}


int QSPI_W25Q64JV_Reset() {

    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_ENABLE_RESET,
    };

    if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
    cmd.Instruction = W25Q64JV_RESET_DEVICE;
    if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK) {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_EraseSector(uint32_t SectorAddress) {
    if (QSPI_W25Q64JV_WriteEnable() != QSPI_OK) {
        return QSPI_ERROR;
    }

    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_ERASE_SECTOR,
        .AddressMode = QSPI_ADDRESS_1_LINE,
        .Address = SectorAddress,
        .AddressSize = QSPI_ADDRESS_24_BITS,
    };

    if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK) {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size) {
    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK) {
        return QSPI_ERROR;
    }

    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_INPUT_FAST_READ,
        .AddressMode = QSPI_ADDRESS_4_LINES,
        .Address = ReadAddr,
        .AddressSize = QSPI_ADDRESS_24_BITS,
        .AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES,
        .AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS,
        .AlternateBytes = 0xF0,     //datasheet p22
        .DataMode = QSPI_DATA_4_LINES,
        .DummyCycles = 4,
        .NbData = Size,
    };

    if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK) {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_PageProgram(uint8_t *pData, uint32_t WriteAddr, uint32_t Size) {
    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK) {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_WriteEnable() != QSPI_OK) {
        return QSPI_ERROR;
    }

    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_PAGE_PROGRAM,
        .AddressMode = QSPI_ADDRESS_1_LINE,
        .Address = WriteAddr,
        .AddressSize = QSPI_ADDRESS_24_BITS,
        .DataMode = QSPI_DATA_1_LINE,
        .DummyCycles = 0,
        .NbData = Size,
    };
    if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Transmit(&hqspi, pData, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    if (QSPI_W25Q64JV_AutoPollingMemReady(HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK) {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

/*
 * 必须确保写的地址范围内的数据全部为0xFF
 * 具有自动换页的功能
 */
int QSPI_W25Q64JV_Write_NoCheck(uint8_t *pData, uint32_t WriteAddr, uint32_t Size) {
    int ret = QSPI_OK;
    uint32_t pageremain = W25Q64JV_PAGE_SIZE - WriteAddr % W25Q64JV_PAGE_SIZE;
    if (Size <= pageremain) {
        pageremain = Size;
    }
    while (1) {
        ret = QSPI_W25Q64JV_PageProgram(pData, WriteAddr, pageremain);
        if (ret != QSPI_OK || Size == pageremain) break;
        pData += pageremain;
        WriteAddr += pageremain;
        Size -= pageremain;
        pageremain = Size > W25Q64JV_PAGE_SIZE ? W25Q64JV_PAGE_SIZE : Size;
    }
    return ret;
}

int QSPI_W25Q64JV_Write(uint8_t *pData, uint32_t WriteAddr, uint32_t Size) {
    int ret = QSPI_OK;
    uint32_t secpos = WriteAddr / W25Q64JV_SECTOR_SIZE;
    uint16_t secoff = WriteAddr % W25Q64JV_SECTOR_SIZE;
    uint16_t secremain = W25Q64JV_SECTOR_SIZE - secoff;
    uint32_t i;
    if (Size <= secremain) secremain = Size;
    while (1) {
        ret = QSPI_W25Q64JV_Read(w25q64jv_buf, secpos * W25Q64JV_SECTOR_SIZE, W25Q64JV_SECTOR_SIZE);  //读出整个扇区的内容
        if (ret != QSPI_OK) break;
        for (i = 0; i < secremain; i++) {
            if (w25q64jv_buf[secoff + i] != 0xFF) break;
        }
        if (i < secremain) {
            ret = QSPI_W25Q64JV_EraseSector(secpos * W25Q64JV_SECTOR_SIZE);
            if (ret != QSPI_OK) break;
            memcpy(w25q64jv_buf + secoff, pData, secremain);
            ret = QSPI_W25Q64JV_Write_NoCheck(w25q64jv_buf, secpos * W25Q64JV_SECTOR_SIZE, W25Q64JV_SECTOR_SIZE);
            if (ret != QSPI_OK) break;
        } else {
            ret = QSPI_W25Q64JV_Write_NoCheck(pData, WriteAddr, secremain);
            if (ret != QSPI_OK) break;
        }
        if (Size == secremain) break;
        secpos++;
        secoff = 0;
        pData += secremain;
        WriteAddr += secremain;
        Size -= secremain;
        secremain = (Size - W25Q64JV_SECTOR_SIZE) ? W25Q64JV_SECTOR_SIZE : Size;
    }
    return ret;
}

int QSPI_W25Q64JV_EnableMemoryMappedMode() {
    QSPI_MemoryMappedTypeDef mem_mapped_cfg = {
        .TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE,
    };

    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_INPUT_FAST_READ,
        .AddressMode = QSPI_ADDRESS_4_LINES,
        .Address = 0,
        .AddressSize = QSPI_ADDRESS_24_BITS,
        .AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES,
        .AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS,
        .AlternateBytes    = 0xf0, //datasheet p22
        .DataMode = QSPI_DATA_4_LINES,
        .DummyCycles = 4,
        .NbData = 0,
    };
    if (HAL_QSPI_MemoryMapped(&hqspi, &cmd, &mem_mapped_cfg) != HAL_OK) {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int QSPI_W25Q64JV_DeviceID(uint8_t id[2]) {
    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_DEVICE_ID,
        .AddressMode = QSPI_ADDRESS_1_LINE,
        .Address = 0,
        .AddressSize = QSPI_ADDRESS_24_BITS,
        .DataMode = QSPI_DATA_1_LINE,
        .NbData = 2,
    };

    if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, id, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }
    return QSPI_OK;
}

int QSPI_W25Q64JV_IDNumber(uint8_t id[8]) {
    QSPI_CommandTypeDef cmd = {
        .InstructionMode = QSPI_INSTRUCTION_1_LINE,
        .Instruction = W25Q64JV_ID_NUMBER,
        .DataMode = QSPI_DATA_1_LINE,
        .DummyCycles = 4,
        .NbData = 8,
    };

    if (HAL_QSPI_Command(&hqspi, &cmd, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, id, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}





/* 开启间接模式并检测前4k是否正常
 * 开启内存映射模式测试是否可以正常读取
 * 退出内存映射模式
 */
 void bsp_qspi_init(void)
{
    uint8_t id[2];
    
    QSPI_W25Q64JV_Reset(); /*初始化复位*/
  
    if (QSPI_OK != QSPI_W25Q64JV_DeviceID(id)) {
        while (1);
			
    }
    printf("  qspi w25q64jv id =%x%X\r\n",id[0],id[1]);
    
    if((id[0]==0xef)&&(id[1]==0x16)){
        printf("  qspi w25q64jv id is ok!!! \r\n");
    }
    else {
        printf("  qspi w25q64jv id is err!!! \r\n");
        return;
    }
    
 
    
   
        // 如果对比成功，再进入内存映射模式进行对比
        QSPI_W25Q64JV_EnableMemoryMappedMode();/*开启内存映射模式*/

       
}



