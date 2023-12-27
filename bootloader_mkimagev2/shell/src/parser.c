/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 */
#include <stdio.h>
#include <string.h>
#include "types.h"
#include "command.h"
#include "interface_cmd.h"
#define FINSH_ARG_MAX    8

 
 static int msh_split(char *cmd, rt_size_t length, char *argv[FINSH_ARG_MAX])
 {
	 char *ptr;
	 rt_size_t position;
	 rt_size_t argc;
	 rt_size_t i;
 
	 ptr = cmd;
	 position = 0; argc = 0;
 
	 while (position < length)
	 {
		 /* strip bank and tab */
		 while ((*ptr == ' ' || *ptr == '\t') && position < length)
		 {
			 *ptr = '\0';
			 ptr ++; position ++;
		 }
 
		 if(argc >= FINSH_ARG_MAX)
		 {
			 putstr("Too many args ! We only Use:\n");
			 for(i = 0; i < argc; i++)
			 {
				 putstr(argv[i]);
			 }
			 putstr("\r\n");
			 break;
		 }
 
		 if (position >= length) break;
 
		 /* handle string */
		 if (*ptr == '"')
		 {
			 ptr ++; position ++;
			 argv[argc] = ptr; argc ++;
 
			 /* skip this string */
			 while (*ptr != '"' && position < length)
			 {
				 if (*ptr == '\\')
				 {
					 if (*(ptr + 1) == '"')
					 {
						 ptr ++; position ++;
					 }
				 }
				 ptr ++; position ++;
			 }
			 if (position >= length) break;
 
			 /* skip '"' */
			 *ptr = '\0'; ptr ++; position ++;
		 }
		 else
		 {
			 argv[argc] = ptr;
			 argc ++;
			 while ((*ptr != ' ' && *ptr != '\t') && position < length)
			 {
				 ptr ++; position ++;
			 }
			 if (position >= length) break;
		 }
	 }
 
	 return argc;
 }


 static int _msh_exec_cmd(char *cmd, rt_size_t length, int *retp)
 {
	 int argc;
	 rt_size_t cmd0_size = 0;
	 struct command *command_t;

	 char *argv[FINSH_ARG_MAX];
	 char cmd_name[20];
	 int i;
 
 	 /* find the size of first command : load a b c */
	 while ((cmd[cmd0_size] != ' ' && cmd[cmd0_size] != '\t') && cmd0_size < length)
		 cmd0_size ++;
	 if (cmd0_size == 0)
		 return -1;

	 if (cmd0_size > 20)
	 	cmd0_size = 20;
	 for (i = 0; i < cmd0_size; i++)
	 	cmd_name[i] = cmd[i];
	 cmd_name[i] = '\0';
 
	 command_t = find_cmd(cmd_name);
	 if (command_t == RT_NULL)
		 return -1;
 
	 /* split arguments */
	 memset(argv, 0x00, sizeof(argv));
	 argc = msh_split(cmd, length, argv);
	 if (argc == 0)
		 return -1;
 
	 /* exec this command */
	 *retp = command_t->function(argc, argv);
	 return 0;
 }
 int msh_exec(char *cmd, rt_size_t length)
 {
	 int cmd_ret;
 
	 /* strim the beginning of command */
	 while (*cmd  == ' ' || *cmd == '\t')
	 {
		 cmd++;
		 length--;
	 }
 
	 if (length == 0)
		 return 0;
 
	 /* Exec sequence:
	  * 1. built-in command
	  * 2. module(if enabled)
	  */
	 if (_msh_exec_cmd(cmd, length, &cmd_ret) == 0)
	 {
		 return cmd_ret;
	 }
 
	 /* truncate the cmd at the first space. */
	 {
		 char *tcmd;
		 tcmd = cmd;
		 while (*tcmd != ' ' && *tcmd != '\0')
		 {
			 tcmd++;
		 }
		 *tcmd = '\0';
	 }
	 putstr(cmd);
	 putstr(" command not found.\r\n");
	 return -1;
 }

