
#include "uart.h"
#include "exception.h"
#include "shell.h"

typedef unsigned int __be32;
typedef		unsigned char		uint8_t;

#define IH_MAGIC	0x27051956	/* Image Magic Number		*/
#define IH_NMLEN	32	/* Image Name Length		*/

typedef struct image_header {
	__be32		ih_magic;	/* Image Header Magic Number	*/
	__be32		ih_hcrc;	/* Image Header CRC Checksum	*/
	__be32		ih_time;	/* Image Creation Timestamp	*/
	__be32		ih_size;	/* Image Data Size		*/
	__be32		ih_load;	/* Data	 Load  Address		*/
	__be32		ih_ep;		/* Entry Point Address		*/
	__be32		ih_dcrc;	/* Image Data CRC Checksum	*/
	uint8_t		ih_os;		/* Operating System		*/
	uint8_t		ih_arch;	/* CPU architecture		*/
	uint8_t		ih_type;	/* Image Type			*/
	uint8_t		ih_comp;	/* Compression Type		*/
	uint8_t		ih_name[IH_NMLEN];	/* Image Name		*/
} image_header_t;

unsigned int be32_to_cpu(unsigned int x)
{
	unsigned char *p = (unsigned char *)&x;
	unsigned int le;
	le = (p[0] << 24) + (p[1] << 16) + (p[2] << 8) + (p[3]);
	return le;
}

extern void start_app(unsigned int new_vector);

void delay(int d)
{
	while(d--);
}

void copy_app(int *from, int *to, int len)
{
	// 从哪里到哪里, 多长 ?
	int i;
	for (i = 0; i < len/4+1; i++)
	{
		to[i] = from[i];
	}
}


void relocate_and_start_app(unsigned int pos)
{
	image_header_t *head;
	unsigned int load;
	unsigned int size;
	unsigned int new_pos = pos+sizeof(image_header_t);
	
	/* 读出头部 */
	head = (image_header_t *)pos;
	
	/* 解析头部 */
	load = be32_to_cpu(head->ih_load);
	size = be32_to_cpu(head->ih_size);
	
	putstr("load = ");
	puthex(load);
	putstr("\r\n");

	putstr("size = ");
	puthex(size);
	putstr("\r\n");
		
	/* 把程序复制到RAM */
	copy_app((int *)new_pos, (int *)load, size);
	
	/* 跳转执行 */
	set_new_vector(load);
	start_app(load);
}

int mymain()
{
	unsigned int app_pos = 0x08040000;	
	
	uart_init();

	putstr("\r\nbootloader\r\n");
	
	shell();
	
	/* start app */
	relocate_and_start_app(app_pos);
	
	//start_app(new_vector);
	
	return 0;
}
