#include "bootloader.h"
#include "stdint.h"
#include "bsp_qspi.h"

/*
QSPI FALSH 

主区
APP:            0X0000 0000 ---> 0X003F EFFF      size : 0X3F F000    4M-4K
PARAMENT:       0X003F F000 ---> 0X003F FFFF      SIZE : 0X1000       4K

备份区
BACK APP1:      0X0040 0000 ---> 0X007F EFFF      size : 0X3F F000    4M-4K
BACK PARAMENT:  0X007F F000 ---> 0X007F FFFF      SIZE : 0X1000       4K

1.数据为空状态  检测主区与备份区是否有CRC   都无CRC读取SD卡数据，未检测到SD卡则进入shell模式等待rz
   有数据则检查是否有版本需要更新，
2.需要更新
3.无需更新


*/

	 
	 
	 

 /* 获取系统参数区数据 */
int get_qspi_falsh_sys_parameny(uint32_t address){
	
	





}




int read_qspi_falsh_data(){
	  
   



}










uint32_t bootloade( ){
    

	   
	   

	



 return 0;

}


