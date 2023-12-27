/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    cryp.c
  * @brief   This file provides code for the configuration
  *          of the CRYP instances.
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
#include "cryp.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

CRYP_HandleTypeDef hcryp;
__ALIGN_BEGIN static const uint32_t pKeyCRYP[8] __ALIGN_END = {
                            0x12345678,0x12345678,0x12345678,0x12345678,0x12345678,0x12345678,0x12345678,0x12345678};
__ALIGN_BEGIN static const uint32_t pInitVectCRYP[4] __ALIGN_END = {
                            0x12345678,0x12345678,0x12345678,0x12345678};

/* CRYP init function */
void MX_CRYP_Init(void)
{

  /* USER CODE BEGIN CRYP_Init 0 */

  /* USER CODE END CRYP_Init 0 */

  /* USER CODE BEGIN CRYP_Init 1 */

  /* USER CODE END CRYP_Init 1 */
  hcryp.Instance = CRYP;
  hcryp.Init.DataType = CRYP_DATATYPE_32B;
  hcryp.Init.KeySize = CRYP_KEYSIZE_256B;
  hcryp.Init.pKey = (uint32_t *)pKeyCRYP;
  hcryp.Init.pInitVect = (uint32_t *)pInitVectCRYP;
  hcryp.Init.Algorithm = CRYP_AES_CBC;
  if (HAL_CRYP_Init(&hcryp) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRYP_Init 2 */

  /* USER CODE END CRYP_Init 2 */

}

void HAL_CRYP_MspInit(CRYP_HandleTypeDef* crypHandle)
{

  if(crypHandle->Instance==CRYP)
  {
  /* USER CODE BEGIN CRYP_MspInit 0 */

  /* USER CODE END CRYP_MspInit 0 */
    /* CRYP clock enable */
    __HAL_RCC_CRYP_CLK_ENABLE();
  /* USER CODE BEGIN CRYP_MspInit 1 */

  /* USER CODE END CRYP_MspInit 1 */
  }
}

void HAL_CRYP_MspDeInit(CRYP_HandleTypeDef* crypHandle)
{

  if(crypHandle->Instance==CRYP)
  {
  /* USER CODE BEGIN CRYP_MspDeInit 0 */

  /* USER CODE END CRYP_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CRYP_CLK_DISABLE();
  /* USER CODE BEGIN CRYP_MspDeInit 1 */

  /* USER CODE END CRYP_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
