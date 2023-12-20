/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2006-04-30     Bernard      the first version for FinSH
 * 2006-05-08     Bernard      change finsh thread stack to 2048
 * 2006-06-03     Bernard      add support for skyeye
 * 2006-09-24     Bernard      remove the code related with hardware
 * 2010-01-18     Bernard      fix down then up key bug.
 * 2010-03-19     Bernard      fix backspace issue and fix device read in shell.
 * 2010-04-01     Bernard      add prompt output when start and remove the empty history
 * 2011-02-23     Bernard      fix variable section end issue of finsh shell
 *                             initialization when use GNU GCC compiler.
 * 2016-11-26     armink       add password authentication
 * 2018-07-02     aozima       add custom prompt support.
 */



#include "command.h"
#include "uart.h"
#include "string.h"
#include "flash_cmd.h"

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


