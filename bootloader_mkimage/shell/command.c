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
#include "string.h"

extern struct command help_cmd;
extern struct command rz_cmd;
extern struct command go_cmd;


static struct command * g_commands[] = {
	&help_cmd,
	&rz_cmd,
	&go_cmd,
	NULL
};

struct command ** get_cmds(void)
{
	return g_commands;
}

struct command *find_cmd(char *name)
{
	int i = 0;
	while (g_commands[i])
	{
		if (strcmp(g_commands[i]->name, name) == 0)
			return g_commands[i];
		i++;
	}
	return NULL;
}














