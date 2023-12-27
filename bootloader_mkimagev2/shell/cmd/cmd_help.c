#include <stdio.h>
#include <string.h>
#include "command.h"

static int help_f(int argc, char **argv)
{
	int i = 0;
	struct command **cmds;
	struct command *cmd;

	cmds = get_cmds();
	
	if (argc == 1)
	{
		while (cmds[i])
		{
			putstr(cmds[i]->name);
			putstr("\t\t - \t");
			putstr(cmds[i]->short_help);
			i++;
		}
	}
	else if(argc == 2)
	{
		/* argv[0] = "help"
		 * argv[1] = "command_name"
		 */
		cmd = find_cmd(argv[1]);
		if (cmd)
		{
			putstr(cmd->name);
			putstr("\r\n");
			putstr(cmd->long_help);
		}
		else
		{
			putstr("can not find command : ");
			putstr(argv[1]);
			putstr("\r\n");
		}
	}
	
	return 0;
}

struct command help_cmd = {
	"help",
		
	"list all commands\r\n",
	
	"Usage: \r\n"
	"       help, list all commands\r\n"
	"       help command, show usage of 'command'\r\n",

	help_f,
};















