

# 程序运行在外部sdram

- 使用bootloader将app拷贝进sdram再跳转运行

STM32默认的SDRAM back1地址为0XC0000000,但是此区域默认不支持指令访问，看其他的帖子配置MPU以支持该区域指令访问，我尝试后还是不行（或许可以，但debug发现不了问题），但看到有一种方法可将SDRAM映射到支持指令访问区域，试试看

## 

![image-20231228163600992](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231228163600992.png)

![image-20231228163716798](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231228163716798.png)

![image-20231228163817813](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231228163817813.png)

- 0x60000000-0x9FFFFFFF支持指令访问（QSPI默认所在地址是0x90000000也在其中）
- 0xA0000000-0xDFFFFFFF 不支持指令访问



![image-20231228165202461](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231228165202461.png)

![image-20231228165340290](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231228165340290.png)

FMC 存储区域映射可通过 FMC_BCR1 寄存器中的 BMAP[1:0] 位修改。 

- 将寄存器中的 BMAP[1:0] 位修改为01即可将sdram的地址重映射 在0x60000000



![image-20231228012032214](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231228012032214.png)

```

```











参考链接：

https://blog.csdn.net/qlexcel/article/details/127593161?ops_request_misc=%257B%2522request%255Fid%2522%253A%2522170369520916800227498017%2522%252C%2522scm%2522%253A%252220140713.130102334..%2522%257D&request_id=170369520916800227498017&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~baidu_landing_v2~default-2-127593161-null-null.142^v99^pc_search_result_base4&utm_term=stm32%20sdram%E8%BF%90%E8%A1%8C%E7%A8%8B%E5%BA%8F&spm=1018.2226.3001.4187