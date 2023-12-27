/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 */



#include "command.h"
#include "string.h"

extern struct command help_cmd;
extern struct command rz_cmd;
extern struct command go_cmd;
extern struct command crc32_cmd; 
extern struct command clear_cmd;
extern struct command reboot_cmd;
extern struct command sevent_cmd;
extern struct command printeven_cmd;
extern struct command flash_cmd;

static struct command * g_commands[] = {
	&help_cmd,
	&rz_cmd,
	&go_cmd,
	&crc32_cmd,
	&clear_cmd,
	&reboot_cmd,
	&printeven_cmd,
	&sevent_cmd,
	&flash_cmd,
	
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














