/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-04-30     Bernard      the first version for FinSH
 * 2006-05-08     Bernard      change finsh thread stack to 2048
 * 2006-06-03     Bernard      add support for skyeye
 * 2006-09-24     Bernard      remove the code related with hardware
 * 2010-01-18     Bernard      fix down then up key bug.
 * 2010-03-19     Bernard      fix backspace issue and fix device read in shell.
 * 2010-04-01     Bernard      add prompt output when start and remove the empty history
 * 2011-02-23     Bernard      fix variable section end issue of finsh shell
 *                             initialization when use GNU GCC compiler.
 * 2016-11-26     armink       add password authentication
 * 2018-07-02     aozima       add custom prompt support.
 */


#include <types.h>
#include "stm_flash.h"
#include "flash_cmd.h"
#include "command.h"
#include "uart.h"
#include "string.h"

typedef struct
{
  __IO uint32_t ACR;
  __IO uint32_t KEYR;
  __IO uint32_t OPTKEYR;
  __IO uint32_t SR;
  __IO uint32_t CR;
  __IO uint32_t AR;
  __IO uint32_t RESERVED;
  __IO uint32_t OBR;
  __IO uint32_t WRPR;
} FLASH_TypeDef;

#define FLASH_TYPEPROGRAM_HALFWORD             0x01U  /*!<Program a half-word (16-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_WORD                 0x02U  /*!<Program a word (32-bit) at a specified address.*/
#define FLASH_TYPEPROGRAM_DOUBLEWORD           0x03U  /*!<Program a double word (64-bit) at a specified address*/

#define ROM_START              ((uint32_t)0x08000000)
#define ROM_SIZE               (512 * 1024)
#define ROM_END                ((uint32_t)(ROM_START + ROM_SIZE))
#define STM32_FLASH_END_ADDRESS        ROM_END
#define FLASH_PAGE_SIZE          0x800U

#define PERIPH_BASE           0x40000000UL /*!< Peripheral base address in the alias region */
#define AHBPERIPH_BASE        (PERIPH_BASE + 0x00020000UL)
#define FLASH_R_BASE          (AHBPERIPH_BASE + 0x00002000UL) /*!< Flash registers base address */

#define FLASH_TIMEOUT_VALUE              50000U /* 50 s */

#define FLASH               ((FLASH_TypeDef *)FLASH_R_BASE)



static void delay_us(uint32_t udelay)
{
    __IO uint32_t Delay = udelay * 72 / 8;//(SystemCoreClock / 8U / 1000000U)
    //见stm32f1xx_hal_rcc.c -- static void RCC_Delay(uint32_t mdelay)
    do
    {
        asm("nop");
    } while (Delay --);
}


/**
  * @}
  */

/** @defgroup FLASH_Exported_Functions_Group2 Peripheral Control functions 
 *  @brief   management functions 
 *
@verbatim   
 ===============================================================================
                      ##### Peripheral Control functions #####
 ===============================================================================  
    [..]
    This subsection provides a set of functions allowing to control the FLASH 
    memory operations.

@endverbatim
  * @{
  */

/**
  * @brief  Unlock the FLASH control register access
  * @retval HAL Status
  */
static void stm32_flash_unlock(void)
{
    if((FLASH->CR & FLASH_CR_LOCK) != 0)
    {
        /* Authorize the FLASH Registers access */
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

/**
  * @brief  Locks the FLASH control register access
  * @retval HAL Status
  */
static void stm32_flash_lock(void)
{
  /* Set the LOCK Bit to lock the FLASH Registers access */
  FLASH->CR |= FLASH_CR_LOCK;  
}


/**
  * @brief  Wait for a FLASH operation to complete.
  * @param  Timeout  maximum flash operation timeout
  * @retval HAL Status
  */
static int FLASH_WaitForLastOperation(uint32_t Timeout)
{
  /* Wait for the FLASH operation to complete by polling on BUSY flag to be reset.
     Even if the FLASH operation fails, the BUSY flag will be reset and an error
     flag will be set */
     
  uint32_t status;

    while ((FLASH->SR & FLASH_FLAG_BSY) && Timeout--)
    {
        delay_us(1);
    }

    if (0 == Timeout)
        return -1;  // timeout

    /* Check FLASH End of Operation flag  */
    if (FLASH->SR & FLASH_FLAG_EOP)
    {
        /* Clear FLASH End of Operation pending bit */
        FLASH->SR  = FLASH_FLAG_EOP;
    }
    

    status = FLASH->SR;
    if((status & FLASH_FLAG_WRPERR)  || 
         (status & FLASH_FLAG_OPTVERR) || 
         (status & FLASH_FLAG_PGERR))
    {
        return -1;
    }

    /* There is no error flag set */
    return 0;
}


/**
  * @brief  Program a half-word (16-bit) at a specified address.
  * @param  Address specify the address to be programmed.
  * @param  Data    specify the data to be programmed.
  * @retval None
  */
static void FLASH_Program_HalfWord(uint32_t Address, uint16_t Data)
{
    /* Proceed to program the new data */
    FLASH->CR |= FLASH_CR_PG;

    /* Write data in the address */
    *(__IO uint16_t*)Address = Data;
}

/**
  * @brief  Erase the specified FLASH memory page
  * @param  PageAddress FLASH page to erase
  *         The value of this parameter depend on device used within the same series      
  * 
  * @retval None
  */
static void FLASH_PageErase(uint32_t PageAddress)
{

    /* Proceed to erase the page */
    FLASH->CR |= FLASH_CR_PER;
    FLASH->AR = PageAddress;
    FLASH->CR |= FLASH_CR_STRT;
}



/**
  * @brief  Program halfword, word or double word at a specified address
  * @note   The function HAL_FLASH_Unlock() should be called before to unlock the FLASH interface
  *         The function HAL_FLASH_Lock() should be called after to lock the FLASH interface
  *
  * @note   If an erase and a program operations are requested simultaneously,    
  *         the erase operation is performed before the program one.
  *  
  * @note   FLASH should be previously erased before new programmation (only exception to this 
  *         is when 0x0000 is programmed)
  *
  * @param  TypeProgram:  Indicate the way to program at a specified address.
  *                       This parameter can be a value of @ref FLASH_Type_Program
  * @param  Address:      Specifies the address to be programmed.
  * @param  Data:         Specifies the data to be programmed
  * 
  * @retval int HAL Status
  */
static int HAL_FLASH_Program(uint32_t TypeProgram, uint32_t Address, uint64_t Data)
{
    int status = -1;
    uint8_t index = 0;
    uint8_t nbiterations = 0;

    /* Process Locked */
    //__HAL_LOCK(&pFlash);


    /* Wait for last operation to be completed */
    status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

    if(status == 0)
    {
        if(TypeProgram == FLASH_TYPEPROGRAM_HALFWORD)
        {
            /* Program halfword (16-bit) at a specified address. */
            nbiterations = 1U;
        }
        else if(TypeProgram == FLASH_TYPEPROGRAM_WORD)
        {
            /* Program word (32-bit = 2*16-bit) at a specified address. */
            nbiterations = 2U;
        }
        else
        {
            /* Program double word (64-bit = 4*16-bit) at a specified address. */
            nbiterations = 4U;
        }

        for (index = 0U; index < nbiterations; index++)
        {
            FLASH_Program_HalfWord((Address + (2U*index)), (uint16_t)(Data >> (16U*index)));

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

            /* If the program operation is completed, disable the PG Bit */
            FLASH->CR &= ~FLASH_CR_PG;
            /* In case of error, stop programation procedure */
            if (status != 0)
            {
                break;
            }
        }
    }

    /* Process Unlocked */
    //__HAL_UNLOCK(&pFlash);

    return status;
}


static unsigned int stm32_flash_read(unsigned char *buf, unsigned int offset, unsigned int size)
{
    memcpy(buf, (const void *)offset, size);
    return size;
}

static unsigned int stm32_flash_write(unsigned char *buf, unsigned int offset, unsigned int size)
{
    unsigned int end_addr   = offset + size;
    unsigned int start_addr  = offset;

    if (offset % 4 != 0)
    {
        putstr("write addr must be 4-byte alignment\r\n");
        return 0;
    }

    if ((end_addr) > STM32_FLASH_END_ADDRESS)
    {
        putstr("write outrange flash size!\r\n");
        return 0;
    }

    stm32_flash_unlock();

    while (offset < end_addr)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, offset, *((rt_uint32_t *)buf)) == 0)
        {
            if (*(uint32_t *)offset != *(uint32_t *)buf)
            {
                break;
            }
            offset += 4;
            buf  += 4;
        }
        else
        {
            break;
        }
    }

    stm32_flash_lock();

    return offset - start_addr;
}


static unsigned int stm32_flash_erase(unsigned int offset, unsigned int size)
{
    unsigned int address = offset;

	stm32_flash_unlock();
	
    /* Wait for last operation to be completed */
    int status = FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);
    
    /* Wait for last operation to be completed */
    if (status == 0)
    {

        /* Erase page by page to be done*/
        for(address = offset;
            address < STM32_FLASH_END_ADDRESS;
            address += FLASH_PAGE_SIZE)
        {   
            FLASH_PageErase(address);

            /* Wait for last operation to be completed */
            status = FLASH_WaitForLastOperation((uint32_t)FLASH_TIMEOUT_VALUE);

            /* If the erase operation is completed, disable the PER Bit */
            FLASH->CR &= ~FLASH_CR_PER;

            if (status != 0)
            {
                /* In case of error, stop erase procedure and return the faulty address */
                break;
            }
        }
    }

	stm32_flash_lock();

      return address - offset;
}


static struct flash_ops stm32_flash = {
    "stm32_flash",
    stm32_flash_read,
    stm32_flash_write,
    stm32_flash_erase,
};

struct flash_ops *get_flash(void)
{
    return &stm32_flash;
}

