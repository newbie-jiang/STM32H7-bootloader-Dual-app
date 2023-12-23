## 硬件CRC配置 以及软硬件速度对比



![image-20231223161925821](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223161925821.png)

使用CUBEMX配置默认使用的是CRC32，从库中可以看出这一点

![image-20231223162013619](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223162013619.png)

![image-20231223162053295](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223162053295.png)

HAL库提供了以下两个计算函数

![image-20231223162239058](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223162239058.png)

- HAL_CRC_Accumulate(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength);  

  这个函数用于在已有的CRC校验结果的基础上累积（accumulate）新的数据块。如果你需要分多次处理数据，比如将数据分成几个块，然后分别进行CRC计算，最后将这些计算结果合并，就可以使用这个函数。

- HAL_CRC_Calculate(CRC_HandleTypeDef *hcrc, uint32_t pBuffer[], uint32_t BufferLength);

   这个函数用于对给定的数据块进行一次完整的CRC计算。如果你有整个数据块，并且想要一次性计算CRC，那么可以使用这个函数。

CRC16配置参考

https://blog.csdn.net/qq_20553613/article/details/133052005?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522170331635116800215093910%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=170331635116800215093910&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~times_rank-2-133052005-null-null.142^v96^pc_search_result_base4&utm_term=stm32%20%E7%A1%AC%E4%BB%B6crc&spm=1018.2226.3001.4187





## 软件CRC32

```C
#define CRC32_POLYNOMIAL 0xEDB88320u
//传入参数：                   数据首地址     数据大小
static uint32_t calculate_crc32(const void *data, size_t size) {
    const uint8_t *byteData = (const uint8_t *)data;
    uint32_t crc = 0xFFFFFFFFu;

    for (size_t i = 0; i < size; ++i) {
        crc ^= byteData[i];
        for (int j = 0; j < 8; ++j) {
            crc = (crc >> 1) ^ ((crc & 1) ? CRC32_POLYNOMIAL : 0);
        }
    }

    return ~crc;
}
```





## 软硬件测速对比：

平台使用STM32H7，内存使用外扩的SDRAM

对比方法

- 使用同一块内存区域校验软硬件CRC32,校验前后IO电平翻转,示波器捕获，该方法比较精确

  ```c
  static int crc32(int argc, char **argv) {
      if (argc != 3) {
          fprintf(stderr, "Usage: crc32 <start_address> <size_in_bytes>\r\n");
          return 1;  // 返回错误码
      }
  
      // 从命令行参数中获取起始地址和大小
      uint32_t dataStartAddress;
      size_t dataSizeInBytes;
  		
  
      if (sscanf(argv[1], "%x", &dataStartAddress) != 1) {
          fprintf(stderr, "Invalid start address\r\n");
          return 1;
      }
  
      if (sscanf(argv[2], "%zu", &dataSizeInBytes) != 1) {
          fprintf(stderr, "Invalid size\r\n");
          return 1;
      }
  		
  		
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
      // 硬件 CRC32
      size_t hard_crc32Result = HAL_CRC_Calculate(&hcrc,( void *)dataStartAddress,dataSizeInBytes);
      hard_crc32Result ^= 0xffffffffU;
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
  		
  	HAL_Delay(10);
  		
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);
      // 软件 CRC32
      size_t soft_crc32Result = calculate_crc32((const void *)dataStartAddress, dataSizeInBytes);
  	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);
  
      // 输出计算得到的 CRC32 值
      printf("hard_crc32Result: 0x%x\r\n", hard_crc32Result);
  	printf("soft_crc32Result: 0x%x\r\n", soft_crc32Result);
  
      return 0;  // 返回成功码
  }
  ```

  

  主要部分

  ![image-20231223163424233](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223163424233.png)

  **128K数据测试结果：**

  ![image-20231223165128463](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223165128463.png)

  

  ![image-20231223164425382](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223164425382.png)

  - 硬件测速在6ms左右
  - 软件测速在25ms左右

​     **2M数据测试结构**

![image-20231223165224031](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223165224031.png)

![image-20231223170238833](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231223170238833.png)

- 硬件在100ms左右
- 软件在410ms左右

总结：

- 对比结果硬件CRC32比软件CRC32快4倍左右

