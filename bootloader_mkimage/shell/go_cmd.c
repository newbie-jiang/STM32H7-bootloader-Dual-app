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

static int go_f(int argc, char **argv)
{
#if 0	
	typedef void (*f_type)(void);
	f_type f;
	f = (f_type)0x20000000;
	f();
#else
//	start_app(0x20000000);
#endif
}

struct command go_cmd = {
	"go",
		
	"jump to 0x20000000 to rum app\r\n",
	
	"Usage: \r\n"
	"       go, jump to 0x20000000 to rum app\r\n",

	go_f,
};















