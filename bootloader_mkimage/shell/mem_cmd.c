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

extern struct flash_ops *get_flash(void);

static int md_f(int argc, char **argv)
{
    unsigned int *buf;
    unsigned int size = 1;
    unsigned int i;

	// fp->read/fp->write/fp->erase
	// md <ram_address>
	if (argc == 2)
	{
		buf = (unsigned int *)str2hex(argv[1]);
	}
    else if (argc == 3)
    {
		buf  = (unsigned int *)str2hex(argv[1]);
        size = str2hex(argv[2]);
    }
    else
    {
        return -1;
    }

	putstr("datas:\r\n");
	for (i = 0; i < size; i++)
	{
		puthex((unsigned int)&buf[i]);
		putstr(" : ");
		puthex(buf[i]);
		putstr("\r\n");
	}
	putstr("\r\n");
	return 0;
}

struct command md_cmd = {
	"md",
		
	"memory display\r\n",
	
	"Usage: \r\n"
	"       md <ram_addr> [size]\r\n",

	md_f,
};


