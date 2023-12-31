/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "cryp.h"
#include "fatfs.h"
#include "quadspi.h"
#include "sdmmc.h"
#include "usart.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include <stdbool.h>
#include "boot.h"
#include "shell.h"
#include "bsp_usart.h"
#include "bsp_sdram.h"
#include "aes.h"
#include "api_information.h"
#include "api_led.h"
#include "api_qspi.h"
#include "api_sdram.h"
#include <stdlib.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */
extern void aes_process(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//SCB->VTOR = 0x90400000;



//清除内存块
void clearMemory(void *startAddress, size_t size) {
    memset(startAddress, 0, size);
}


void process_data_blocks(volatile uint32_t *input_data, volatile uint32_t *encrypted_data,
                         volatile uint32_t *decrypted_data, uint32_t block_size) {
    uint32_t total_size = 0x3ffc; // 总大小
    uint32_t block_count = total_size / block_size; // 数据块的数量

    for (uint32_t block = 0; block < block_count; ++block) {
        uint32_t start_index = block * block_size;
        uint32_t end_index = start_index + block_size;

        /* Encrypt data block */
        if (HAL_OK == HAL_CRYP_Encrypt(&hcryp, (uint32_t *)&input_data[start_index], block_size / 4,
                                       (uint32_t *)&encrypted_data[start_index], HAL_MAX_DELAY)) {
            printf("Encrypt block %d ok\r\n", block);
        } else {
            printf("Encrypt block %d err\r\n", block);
        }

        /* Decrypt data block */
        if (HAL_OK == HAL_CRYP_Decrypt(&hcryp, (uint32_t *)&encrypted_data[start_index], block_size / 4,
                                       (uint32_t *)&decrypted_data[start_index], HAL_MAX_DELAY)) {
            printf("Decrypt block %d ok\r\n", block);
        } else {
            printf("Decrypt block %d err\r\n", block);
        }

        /* Verify the result */
        for (uint32_t i = start_index; i < end_index; ++i) {
            if (input_data[i] != decrypted_data[i]) {
                /* Verification failed */
                printf("Verification error at address 0x%08X\r\n", (unsigned int)&input_data[i]);
                while (1) {
                    /* 在这里添加错误处理 */
                    printf("err\r\n");
                    HAL_Delay(1000);
                }
            }
        }

        printf("Decrypt check block %d success\r\n", block);
    }
}

   extern int test(void)    ;
extern void aes_pro(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
//  FMC_Bank1_R->BTCR[0] = 0x000030D2;
  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
//  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();
  SCB_EnableDCache();
  /* Enable D-Cache---------------------------------------------------------*/
//  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_UART4_Init();
//  MX_QUADSPI_Init();
//  MX_SDMMC1_SD_Init();
//  MX_FATFS_Init();
//  MX_FMC_Init();
//  MX_CRC_Init();
//  MX_CRYP_Init();
  /* USER CODE BEGIN 2 */
//  jump_to_qspi_flash();
	putstr("\r\nbootloader\r\n");

	
	//https://club.rt-thread.org/ask/article/789b576a217a64a7.html
	
//	test_clock_info(&sapi_clock_info);
//	test_chip_id_info();
//  test_api_get_falsh_size();
//	test_led_devive(&sapi_led_driver);
//	test_qspi_flash_driver(&sapi_qspi_flash_driver);
//	tets_sdram_driver(&ssdram_driver);
	
//	
//	 uint8_t cleartext[]  = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
//                             0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
//                             0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
//                             0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };

//	volatile uint32_t *fmc_sdram_remap_address = (volatile uint32_t *)0x000030DB;
//	*fmc_sdram_remap_address |= 0X1<<24;

//	SYSCFG->MEMRMP |= (1<<10);  // 将sdram remap到0x60000000位置才可以使得cpu去执行  
//	uint32_t *memoryStartAddress = (uint32_t *)0x24000000;  // 0x24000000 
//  size_t memorySize = ;  // 20KB
//  clearMemory(memoryStartAddress, memorySize); // 清除内存块
	// 数据起始地址
	
	//ST加密扩展包
//https://www.st.com/en/embedded-software/x-cube-cryptolib.html
//https://www.stmcu.com.cn/ecosystem/app/information-security-SBSFU	

		
   
//		uint8_t arr[32] = { 0x12, 0x34, 0x56, 0x78,0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78,0x12, 0x34, 0x56, 0x78,
//                        0x12, 0x34, 0x56, 0x78,0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78,0x12, 0x34, 0x56, 0x78, };

//    volatile uint32_t *input_data = (volatile uint32_t *)0x24000000;
//    volatile uint32_t *encrypted_data = (volatile uint32_t *)0x24010000;
//    volatile uint32_t *decrypted_data = (volatile uint32_t *)0x24020000;
//    size_t size = sizeof(cleartext);  // 计算数组的字节数
//    
//		
//		
//		uint32_t clean_byte = 1024*512;
//		
//		
//    memset((void *)input_data, 0, size);
//		
//    memcpy((void *)input_data, (void *)cleartext, size);  // 使用 memcpy 复制数组数据

////	
////  
////	
//    /* Encrypt data */
//    if(HAL_OK==HAL_CRYP_Encrypt(&hcryp, (uint32_t *)input_data, sizeof(cleartext)/4, (uint32_t *)encrypted_data, HAL_MAX_DELAY))
//    printf("Encrypt  ok \r\n");
//		else 
//		printf("Encrypt  err \r\n");
//    /* Decrypt data */
//    if(HAL_OK==HAL_CRYP_Decrypt(&hcryp, (uint32_t *)encrypted_data, sizeof(cleartext)/4, (uint32_t *)decrypted_data, HAL_MAX_DELAY))
//    printf("Decrypt  ok \r\n");
//			else 
//		printf("Decrypt  err \r\n");
//    /* Verify the result */

//   /* 验证结果 */
//   for (int i = 0; i < sizeof(cleartext)/4; ++i) {
//      if (input_data[i] != decrypted_data[i]) {
//        /* 验证失败 */
//        while (1) {
//            /* 在这里添加错误处理 */
//            printf("err \r\n");
//					  printf("Verification error at address 0x%08X\r\n", (unsigned int)&input_data[i]);
//            HAL_Delay(1000);
//        }
//    }
//}
//		printf("Decrypt check  success \r\n");
//     aes_process();
		 
     aes_pro();
	   shell();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
//	MPU_InitStruct.BaseAddress      = 0xC0000000;
//	MPU_InitStruct.Size             = MPU_REGION_SIZE_16MB;
//	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
//	MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
//	MPU_InitStruct.IsCacheable      = MPU_ACCESS_NOT_CACHEABLE;
//	MPU_InitStruct.IsShareable      = MPU_ACCESS_SHAREABLE;
//	MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
//	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL0;
//	MPU_InitStruct.SubRegionDisable = 0x00;
//	MPU_InitStruct.DisableExec      = MPU_INSTRUCTION_ACCESS_ENABLE;

//	HAL_MPU_ConfigRegion(&MPU_InitStruct);

//	/*ê1?ü MPU */
//	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);	
/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_32MB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL1;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
