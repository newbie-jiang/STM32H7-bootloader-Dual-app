/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-06-02     Bernard      Add finsh_get_prompt function declaration
 */

#ifndef __FLASH_COMMAND_H__
#define __FLASH_COMMAND_H__

#include "types.h"

struct flash_ops {
  char *name;
  unsigned int (*read)(unsigned char *buf, unsigned int offset, unsigned int size);
  unsigned int (*write)(unsigned char *buf, unsigned int offset, unsigned int size);
  unsigned int (*erase)(unsigned int offset, unsigned int size);
};



#endif

