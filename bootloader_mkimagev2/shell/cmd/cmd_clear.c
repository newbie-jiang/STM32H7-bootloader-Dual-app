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



#include <stdio.h>
#include <string.h>
#include "command.h"



static int clear(int argc, char **argv)
{
  fputs("\x1b[2J\x1b[H", stdout);
}

struct command clear_cmd = {
	"clear",
		
	"clear window display\r\n",
	
	"Usage: \r\n"
	"       clear,clear window display\r\n",

	clear,
};















