/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 */



#include "command.h"
#include "string.h"

extern struct command help_cmd;

static struct command * g_commands[] = {
	&help_cmd,
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














