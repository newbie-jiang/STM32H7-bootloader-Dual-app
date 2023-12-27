
#include "file.h"

static struct stat g_rcv_st;

int fileno(FILE *stream)
{
	return 123;
}

int fstat(int fd, struct stat *statbuf)
{
	*statbuf = g_rcv_st;
	return 0;
}

FILE *popen(const char *command, const char *type)
{
	return (FILE*)1;
}


int pclose(FILE *stream)
{
	return 0;
}

unsigned int sleep(unsigned int seconds)
{
	return 0;
}

FILE *fopen2(const char *pathname, const char *mode)
{
	g_rcv_st.datas   = (unsigned char *)0x24000000;
//	g_rcv_st.datas   = (unsigned char *)0xc0000000;
	g_rcv_st.offset  = 0;
	g_rcv_st.st_size = 0;
	return (FILE *)1;
}

size_t fwrite2(const void *ptr, size_t size, size_t nmemb,
			  FILE *stream)
{
	unsigned char *pdata = (unsigned char *)ptr;
	int i;
	for (i = 0; i < size; i++)
		g_rcv_st.datas[g_rcv_st.offset++] = pdata[i];

	g_rcv_st.st_size = g_rcv_st.offset;
	return size;
}

			  
int putc2(int c, FILE *stream)
{
	g_rcv_st.datas[g_rcv_st.offset++] = c;
	g_rcv_st.st_size = g_rcv_st.offset;
	return c;
}


int get_file_size(void){

return g_rcv_st.st_size;

}



