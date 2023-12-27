
#include <stdio.h>
#include <string.h>
#include "command.h"
#include "interface_cmd.h"


unsigned int str2hex(const char* s)
{
	unsigned int sum=0;
    unsigned char c;
    unsigned int val;
	while ( *s == ' '  ||  *s == '\t') s++;

    if (*s == '0')s++;
    if (*s == 'x')s++;
    if (*s == 'X')s++;    

    c = *s;
	while (c)
	{
        if (c >= '0' && c <= '9')
            val = c - '0';
        else if (c >= 'a' && c <= 'z')
            val = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z')
            val = c - 'A' + 10;
		sum = sum * 16 + val;
		++s;
        c = *s;
	}
	return sum;
}

static int flash_f(int argc, char **argv)
{
	struct flash_ops *fp = get_flash();
	
    unsigned char *buf;
    unsigned int offset;
    unsigned int size = 0;
    unsigned int ret = 1;

	// fp->read/fp->write/fp->erase
	if (argc == 5)
	{
        buf    = (unsigned char *)str2hex(argv[2]);
        offset = str2hex(argv[3]);
        size   = str2hex(argv[4]);
	}
    else if (argc == 4)
    {
        offset = str2hex(argv[2]);
        size   = str2hex(argv[3]);
    }
    else
    {
        return -1;
    }

    if (!strcmp(argv[1], "read"))
        ret = fp->read(buf, offset, size);
    else if (!strcmp(argv[1], "write"))
        ret = fp->write(buf, offset, size);
    else if (!strcmp(argv[1], "erase"))
        ret = fp->erase(offset, size);
    else 
    {
        return -1;
    }

    return (ret == size) ? 0 : -1;
}

struct command flash_cmd = {
	"flash",
		
	"read/write/erase flash\r\n",
	
	"Usage: \r\n"
	"       flash read <ram_addr> <flash_addr> <size>\r\n"
	"       flash write <ram_addr> <flash_addr> <size>\r\n"
	"       flash erase <flash_addr> <size>\r\n",

	flash_f,
};


