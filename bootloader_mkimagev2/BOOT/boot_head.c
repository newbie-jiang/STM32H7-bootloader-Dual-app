
#include "string.h"
#include "stdio.h"
#include "boot_head.h"
#include "bsp_sd.h"


/* mkimage头部数据解析 */


/* 4字节大端字节序转小端字节序 */
uint32_t my_ntohl(uint32_t netlong) {
    return ((netlong & 0xFF000000) >> 24) |
           ((netlong & 0x00FF0000) >> 8) |
           ((netlong & 0x0000FF00) << 8) |
           ((netlong & 0x000000FF) << 24);
}



/*打印头部信息*/
void printf_app_head(image_header_t *pimage_header_t){

	# if(0)   //用于调试
   printf("ih_magic in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_magic));
	 printf("ih_hcrc in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_hcrc));
	 printf("ih_time in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_time));
	 printf("ih_size in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_size));
	 printf("ih_load in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_load));
	 printf("ih_ep in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_ep));
	 printf("ih_dcrc in hexadecimal: 0x%x\r\n", my_ntohl(pimage_header_t->ih_dcrc));
	
	 printf("ih_os in hexadecimal: 0x%x\r\n",   pimage_header_t->ih_os);
	 printf("ih_arch in hexadecimal: 0x%x\r\n", pimage_header_t->ih_arch);
	 printf("ih_type in hexadecimal: 0x%x\r\n", pimage_header_t->ih_type);
	 printf("ih_comp in hexadecimal: 0x%x\r\n", pimage_header_t->ih_comp);
	 printf("| ih_name is: ");
   for (int i = 0; i < IH_NMLEN; i++) 
	 printf("%c", (unsigned char)pimage_header_t->ih_name[i]);  printf("\r\n");
	#else
	 printf("| ih_name   |");
   for (int i = 0; i < IH_NMLEN; i++) 
	 printf("%c", (unsigned char)pimage_header_t->ih_name[i]);  printf("\r\n");
	
	 printf("| ih_size   |  %u Byte    \r\n", my_ntohl(pimage_header_t->ih_size));
	 printf("| ih_load   |  0x%x       \r\n", my_ntohl(pimage_header_t->ih_load));
	 printf("| ih_ep     |  0x%x       \r\n", my_ntohl(pimage_header_t->ih_ep));
	 printf("| ih_hcrc   |  0x%x       \r\n", my_ntohl(pimage_header_t->ih_hcrc)); //头部crc
	 printf("| ih_dcrc   |  0x%x       \r\n", my_ntohl(pimage_header_t->ih_dcrc)); //app crc32
	 printf("\r\n");
	#endif
 
}
	

//
uint32_t check_app_head(image_header_t *app_image_header_t, char *app_name) 
{
    int bytesRead = 0;  //获取字符数

    bytesRead = readFileContent(app_name, app_image_header_t, sizeof(image_header_t), 0);

    if (bytesRead != sizeof(image_header_t)) {
			  printf(" boot ahead bytesRead err\r\n "); //读取字符数量不一致
		    return -1;}
    else 
        printf_app_head(app_image_header_t); //打印mkimage 头部信息
		   
		    return 0;	
 }

