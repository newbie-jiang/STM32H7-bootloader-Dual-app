#include "aes.h"
#include "stdio.h"
#include "string.h"
#include "cmox_crypto.h"
#include "stdio.h"
const uint8_t key[] = { 0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
                      0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4 };


const uint8_t iv[]  = { 0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
 

void aes_process(void)
{
	  //明文
	  uint8_t cleartext[]  = { 0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
                             0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
                             0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
                             0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10 };
	
														 									 
														 
														 
	  //打印明文
		printf("cleartext buffer is \r\n");											
		for(int i =0; i<64;i++)
		{
		   printf(" %x" ,cleartext[i]);	
			 if((i+1)%16==0)printf("\r\n");		
		}
		
		printf("\r\n");													
														
														
		struct AES_ctx ctx;

    AES_init_ctx_iv(&ctx, key,  iv);
    AES_CBC_encrypt_buffer(&ctx, cleartext, 64); //加密
		//打印加密内容						
		printf("encrypt buffer is \r\n");											
		for(int i =0; i<64;i++)
		{
		   printf(" %x" ,cleartext[i]);	
			 if((i+1)%16==0)printf("\r\n");		
		}
		
		printf("\r\n");	
		
		

    AES_init_ctx_iv(&ctx, key,  iv);  //此处必须再处使用
		
		//解密
		AES_CBC_decrypt_buffer(&ctx, cleartext, 64);
		//打印解密内容						
		printf("decrypt buffer is \r\n");			
    for(int i =0; i<64;i++)
		{
		   printf(" %x" ,cleartext[i]);	
			 if((i+1)%16==0)printf("\r\n");		
		}	

   
		
				
}




#define FAILED  ((TestStatus)0)
#define PASSED  ((TestStatus)1)
typedef uint32_t TestStatus;




/* Global variables ----------------------------------------------------------*/
/* CBC context handle */
cmox_cbc_handle_t Cbc_Ctx;

volatile TestStatus glob_status = FAILED;
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define CHUNK_SIZE  48u   /* Chunk size (in bytes) when data to encrypt or decrypt are processed by chunk */
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** Extract from NIST Special Publication 800-38A
  * F.2.1 CBC-AES128.Encrypt
    Key 2b7e151628aed2a6abf7158809cf4f3c
    IV 000102030405060708090a0b0c0d0e0f
    Block #1
    Plaintext 6bc1bee22e409f96e93d7e117393172a
    Input Block 6bc0bce12a459991e134741a7f9e1925
    Output Block 7649abac8119b246cee98e9b12e9197d
    Ciphertext 7649abac8119b246cee98e9b12e9197d
    Block #2
    Plaintext ae2d8a571e03ac9c9eb76fac45af8e51
    Input Block d86421fb9f1a1eda505ee1375746972c
    Output Block 5086cb9b507219ee95db113a917678b2
    Ciphertext 5086cb9b507219ee95db113a917678b2
    Block #3
    Plaintext 30c81c46a35ce411e5fbc1191a0a52ef
    Input Block 604ed7ddf32efdff7020d0238b7c2a5d
    Output Block 73bed6b8e3c1743b7116e69e22229516
    Ciphertext 73bed6b8e3c1743b7116e69e22229516
    Block #4
    Plaintext f69f2445df4f9b17ad2b417be66c3710
    Input Block 8521f2fd3c8eef2cdc3da7e5c44ea206
    Output Block 3ff1caa1681fac09120eca307586e1a7
    Ciphertext 3ff1caa1681fac09120eca307586e1a7
  */
__attribute__((aligned(4))) const uint8_t Key[] =
{
  0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
};
__attribute__((aligned(4))) const uint8_t IV[] =
{
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};
__attribute__((aligned(4))) const uint8_t Plaintext[] =
{
  0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
  0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
  0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef,
  0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10
};
const uint8_t Expected_Ciphertext[] =
{
  0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d,
  0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2,
  0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16,
  0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7
};

/* Computed data buffer */
uint8_t Computed_Ciphertext[sizeof(Expected_Ciphertext)];
uint8_t Computed_Plaintext[sizeof(Plaintext)];



void aes_pro(void){
	
	
	
  cmox_cipher_retval_t retval;
  size_t computed_size;
  /* General cipher context */
  cmox_cipher_handle_t *cipher_ctx;
  /* Index for piecemeal processing */
  uint32_t index;

  printf("run now");

  /* Initialize cryptographic library */
  if (cmox_initialize(NULL) != CMOX_INIT_SUCCESS)
  {
   printf("err1");
  }

  printf("run now");
  /* Compute directly the ciphertext passing all the needed parameters */
  /* Note: CMOX_AES_CBC_ENC_ALGO refer to the default AES implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_AESFAST_CBC_ENC_ALGO to select the AES fast implementation
   * - CMOX_AESSMALL_CBC_ENC_ALGO to select the AES small implementation
   */
  retval = cmox_cipher_encrypt(CMOX_AES_CBC_ENC_ALGO,                  /* Use AES CBC algorithm */
                               Plaintext, sizeof(Plaintext),           /* Plaintext to encrypt */
                               Key, sizeof(Key),                       /* AES key to use */
                               IV, sizeof(IV),                         /* Initialization vector */
                               Computed_Ciphertext, &computed_size);   /* Data buffer to receive generated ciphertext */

  /* Verify API returned value */
															 
	printf("run now");
  if (retval != CMOX_CIPHER_SUCCESS)
  {
     printf("err2");
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Expected_Ciphertext))
  {
    printf("err3");
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Expected_Ciphertext, Computed_Ciphertext, computed_size) != 0)
  {
     printf("err4");
  }

  /* Compute directly the plaintext passing all the needed parameters */
  /* Note: CMOX_AES_CBC_DEC_ALGO refer to the default AES implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_AESFAST_CBC_DEC_ALGO to select the AES fast implementation
   * - CMOX_AESSMALL_CBC_DEC_ALGO to select the AES small implementation
   */
  retval = cmox_cipher_decrypt(CMOX_AES_CBC_DEC_ALGO,                 /* Use AES CBC algorithm */
                               Expected_Ciphertext, sizeof(Expected_Ciphertext), /* Ciphertext to decrypt */
                               Key, sizeof(Key),                      /* AES key to use */
                               IV, sizeof(IV),                        /* Initialization vector */
                               Computed_Plaintext, &computed_size);   /* Data buffer to receive generated plaintext */

  /* Verify API returned value */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    printf("err5");
  }

  /* Verify generated data size is the expected one */
  if (computed_size != sizeof(Plaintext))
  {
     printf("err6");
  }

  /* Verify generated data are the expected ones */
  if (memcmp(Plaintext, Computed_Plaintext, computed_size) != 0)
  {
     printf("err7");
  }

  /* --------------------------------------------------------------------------
   * MULTIPLE CALLS USAGE
   * --------------------------------------------------------------------------
   */

  /* Construct a cipher context that is configured to perform AES CBC encryption operations */
  /* Note: CMOX_AES_CBC_ENC refer to the default AES implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_AESFAST_CBC_ENC to select the AES fast implementation
   * - CMOX_AESSMALL_CBC_ENC to select the AES small implementation
   */
  cipher_ctx = cmox_cbc_construct(&Cbc_Ctx, CMOX_AES_CBC_ENC);
  if (cipher_ctx == NULL)
  {
    printf("err8");
  }

  /* Initialize the cipher context */
  retval = cmox_cipher_init(cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
     printf("err9");
  }

  /* Setup of the encryption key into the context */
  retval = cmox_cipher_setKey(cipher_ctx, Key, sizeof(Key));  /* AES key to use */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    printf("err10");
  }

  /* Setup of the Initialization Vector (IV) into the context */
  retval = cmox_cipher_setIV(cipher_ctx, IV, sizeof(IV));     /* Initialization vector */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
     printf("err11");
  }

  /* Encrypt the plaintext in multiple steps by appending chunks of CHUNK_SIZE bytes */
  for (index = 0; index < (sizeof(Plaintext) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_cipher_append(cipher_ctx,
                                &Plaintext[index], CHUNK_SIZE,        /* Chunk of plaintext to encrypt */
                                Computed_Ciphertext, &computed_size); /* Data buffer to receive generated chunk
                                                                         of ciphertext */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
    {
       printf("err12");
    }

    /* Verify generated data size is the expected one */
    if (computed_size != CHUNK_SIZE)
    {
       printf("err13");
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&Expected_Ciphertext[index], Computed_Ciphertext, computed_size) != 0)
    {
       printf("err14");
    }
  }
  /* Process with encryption of the last part if needed */
  if (index < sizeof(Plaintext))
  {
    retval = cmox_cipher_append(cipher_ctx,
                                &Plaintext[index],
                                sizeof(Plaintext) - index,              /* Last part of plaintext to encrypt */
                                Computed_Ciphertext, &computed_size);   /* Data buffer to receive generated last
                                                                           part of ciphertext */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
    {
       printf("err15");
    }

    /* Verify generated data size is the expected one */
    if (computed_size != (sizeof(Plaintext) - index))
    {
       printf("err16");
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&Expected_Ciphertext[index], Computed_Ciphertext, computed_size) != 0)
    {
      printf("err17");
    }
  }

  /* Cleanup the context */
  retval = cmox_cipher_cleanup(cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    printf("err18");
  }

  /* Construct a cipher context that is configured to perform AES CBC decryption operations */
  /* Note: CMOX_AES_CBC_DEC refer to the default AES implementation
   * selected in cmox_default_config.h. To use a specific implementation, user can
   * directly choose:
   * - CMOX_AESFAST_CBC_DEC to select the AES fast implementation
   * - CMOX_AESSMALL_CBC_DEC to select the AES small implementation
   */
  cipher_ctx = cmox_cbc_construct(&Cbc_Ctx, CMOX_AES_CBC_DEC);
  if (cipher_ctx == NULL)
  {
     printf("err19");
  }

  /* Initialize the cipher context */
  retval = cmox_cipher_init(cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
     printf("err20");
  }

  /* Setup of the decryption key into the context */
  retval = cmox_cipher_setKey(cipher_ctx, Key, sizeof(Key));  /* AES key to use */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
    printf("err21");
  }

  /* Setup of the Initialization Vector (IV) into the context */
  retval = cmox_cipher_setIV(cipher_ctx, IV, sizeof(IV));     /* Initialization vector */
  if (retval != CMOX_CIPHER_SUCCESS)
  {
     printf("err22");
  }

  /* Decrypt the plaintext in multiple steps by appending chunks of CHUNK_SIZE bytes */
  for (index = 0; index < (sizeof(Expected_Ciphertext) - CHUNK_SIZE); index += CHUNK_SIZE)
  {
    retval = cmox_cipher_append(cipher_ctx,
                                &Expected_Ciphertext[index],
                                CHUNK_SIZE,                           /* Chunk of ciphertext to decrypt */
                                Computed_Plaintext, &computed_size);  /* Data buffer to receive generated
                                                                         chunk of plaintext */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
    {
       printf("err23");
    }

    /* Verify generated data size is the expected one */
    if (computed_size != CHUNK_SIZE)
    {
       printf("err24");
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&Plaintext[index], Computed_Plaintext, computed_size) != 0)
    {
       printf("err25");
    }
  }
  /* Process with encryption of the last part if needed */
  if (index < sizeof(Expected_Ciphertext))
  {
    retval = cmox_cipher_append(cipher_ctx,
                                &Expected_Ciphertext[index],
                                sizeof(Expected_Ciphertext) - index,    /* Last part of ciphertext to decrypt */
                                Computed_Plaintext, &computed_size);    /* Data buffer to receive generated last
                                                                           part of plaintext */

    /* Verify API returned value */
    if (retval != CMOX_CIPHER_SUCCESS)
    {
       printf("err26");
    }

    /* Verify generated data size is the expected one */
    if (computed_size != (sizeof(Expected_Ciphertext) - index))
    {
       printf("err27");
    }

    /* Verify generated data are the expected ones */
    if (memcmp(&Plaintext[index], Computed_Plaintext, computed_size) != 0)
    {
       printf("err28");
    }
  }

  /* Cleanup the handle */
  retval = cmox_cipher_cleanup(cipher_ctx);
  if (retval != CMOX_CIPHER_SUCCESS)
  {
     printf("err29");
  }

  /* No more need of cryptographic services, finalize cryptographic library */
  if (cmox_finalize(NULL) != CMOX_INIT_SUCCESS)
  {
     printf("err30");
  }

  /* Turn on LED2 in an infinite loop in case of AES CBC operations are successful */
	
  //successful
	
	printf(" test lib success");
  glob_status = PASSED;



}





