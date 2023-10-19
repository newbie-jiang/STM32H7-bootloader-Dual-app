## 基于QSPI的双APP+SD卡固件升级方案

在之前实现跳转的基础上做的扩展，参考链接：https://github.com/HDJ1839822/STM32-_bootloader

适用场景：减少维护成本，固件更新只需要将固件下发客户，保留历史版本的APP用于应对可能出现的意外



功能如下：

- QSPI中未识别到APP将主动检测SD卡
- APP的数据校验（两次校验）
- 双APP来回切换（KEY短按）
- 手动升级(KEY长按)
- 记忆启动APP



QSPI使用W25Q64JV     8M字节，划分区域如下(W25Q64按Page擦除，大小4K，较大固件可将擦除区域调大以加快擦除速度)   

![image-20231017205323461](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231017205323461.png)

- APP1 区域存放固件1
- APP2 区域存放固件2
- CRC 为预留区域（该功能目前未实现） 目前固件采用的是逐字节比较校验，对于大固件不适用，效率较低
- SYS Parament 为系统配置参数区域，实现记忆功能与参数调整（便于后续可扩展USART配置参数）



## SD卡数据至QSPI 流程（两次校验）

- SD卡数据写入SDRAM
- 逐字节比较SD与SDRAM固件数据一致性
- SDRAM数据写入QSPI
- 诸字节比较SDRAM与QSPI中数据一致性



使用SDRAM缓存的意义

- 可在SDRAM预处理,检验固件是否合法，增加QSPI使用寿命
- 写入QSPI时出现错误，可以从SDRAM重新获取数据
- 方便扩展OTA,可直接在SDRAM校验，且处理速率较快

缺点：需要两次校验来保证数据准确性，增加了系统的复杂性





## APP1

- 地址0x9000 0000
- 大小0x400000(4MB)

![image-20231017212633802](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231017212633802.png)

![image-20231017212544568](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231017212544568.png)

![image-20231017212603907](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231017212603907.png)

![image-20231017212717228](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231017212717228.png)

## APP2

![image-20231019185758041](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019185758041.png)

![image-20231019185825740](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019185825740.png)

![image-20231019185916261](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019185916261.png)

![image-20231019185950713](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019185950713.png)

## BootLoader

大致逻辑，忽略了一些细节，详细看代码

## 正常启动模式

（上电倒计时内未检测到用户按下按键逻辑部分）

![image-20231019192014985](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019192014985.png)

![image-20231019201524041](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019201524041.png)




![image-20231019202810303](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019202810303.png)



## 短按（切换APP）

![image-20231019200039276](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019200039276.png)

![image-20231019202953413](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019202953410.png)



## 长按（从SD卡更新固件）



![image-20231019201329970](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019201329970.png)

![image-20231019203944060](https://newbie-typora.oss-cn-shenzhen.aliyuncs.com/TyporaJPG/image-20231019203944061.png)

