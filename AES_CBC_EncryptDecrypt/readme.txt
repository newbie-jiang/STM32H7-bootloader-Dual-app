/**
  @page AES_CBC_EncryptDecrypt Cipher example

  @verbatim
  ******************************************************************************
  * @file    Applications/Cipher/AES_CBC_EncryptDecrypt/readme.txt
  * @author  MCD Application Team
  * @brief   Description of the AES_CBC_EncryptDecrypt example
  ******************************************************************************
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file in
  * the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
   @endverbatim

@par Example Description
This example describes how to use the STM32 Cryptographic Library to perform encryption
and decryption using the AES CBC algorithm.

This example demonstrates two ways of using the cryptographic services:
1 - The single call method: algorithm configuration and execution is done via a single API call.
2 - The multiple calls method: algorithm configuration and execution is done in several API calls,
allowing in particular a piecemeal injection of data to process.

For each method, there are two examples provided:
a - An encryption of a known plaintext, followed by the verification of the generated ciphertext
b - An decryption of a known ciphertext, followed by the verification of the generated plaintext

In case of successful operations:
- the yellow led will be turned on
- the global variable glob_status will be set to PASSED
In case of failure:
- the yellow led will be toggled each 250 milliseconds in an infinity loop.
- the global variable glob_status will be set to FAILED

@note NIST vectors examples for AES CBC are taken from:
"Recommendation for Block Cipher Modes of Operation, 2001 Edition"
Available at:
 https://csrc.nist.gov/publications/detail/sp/800-38a/final

@par Keywords

Cryptography, cipher, decipher, AES, AESCBC, Cryptographic

@par Directory contents

  - Cipher/AES_CBC_EncryptDecrypt/Inc/stm32h7xx_hal_conf.h    HAL configuration file
  - Cipher/AES_CBC_EncryptDecrypt/Inc/stm32h7xx_it.h          Interrupt handlers header file
  - Cipher/AES_CBC_EncryptDecrypt/Inc/main.h                        Header for main.c module
  - Cipher/AES_CBC_EncryptDecrypt/Src/stm32h7xx_it.c          Interrupt handlers
  - Cipher/AES_CBC_EncryptDecrypt/Src/stm32h7xx_hal_msp.c     HAL MSP module
  - Cipher/AES_CBC_EncryptDecrypt/Src/main.c                        Main program
  - Cipher/AES_CBC_EncryptDecrypt/Src/system_stm32h7xx.c      STM32H7xx system source file
  - Cipher/AES_CBC_EncryptDecrypt/Src/cmox_low_level.c              CMOX low level services implementation

@par Hardware and Software environment

  - This example runs on STM32H7xx devices.

  - This example has been tested with STM32H743ZI-Nucleo board and can be
    easily tailored to any other supported device and development board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
