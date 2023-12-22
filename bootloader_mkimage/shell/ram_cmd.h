/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-06-02     Bernard      Add finsh_get_prompt function declaration
 */

#ifndef __RAM_COMMAND_H__
#define __RAM_COMMAND_H__

#include "types.h"

struct ram_command {
  char *name;
  char *short_help;
  char *long_help;
  unsigned int (*read)(unsigned char *buf, unsigned int offset, unsigned int size);
  unsigned int (*write)(unsigned char *buf, unsigned int offset, unsigned int size);
  unsigned int (*erase)(unsigned int offset, unsigned int size);
};


//struct command ** get_cmds(void);
//struct command *find_cmd(char *name);


#endif

