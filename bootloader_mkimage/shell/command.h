/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-06-02     Bernard      Add finsh_get_prompt function declaration
 */

#ifndef __COMMAND_H__
#define __COMMAND_H__

#include "types.h"

struct command {
  char *name;         //ÃüÁîÃû³Æ
  char *short_help;   //¶ÌÃüÁîÓï¾ä
  char *long_help;    //³¤ÃüÁîÓï¾ä
  int (*function)(int argc, char **argv); //Ö´ĞĞº¯Êı
};

struct command ** get_cmds(void);
struct command *find_cmd(char *name);


#endif

