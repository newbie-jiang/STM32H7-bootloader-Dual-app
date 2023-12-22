/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-06-02     Bernard      Add finsh_get_prompt function declaration
 */

#ifndef __CRC32_COMMAND_H__
#define __CRC32_COMMAND_H__

#include "types.h"

struct crc32_test {
  char *name;
  unsigned int (*read)(unsigned char *buf, unsigned int offset, unsigned int size);
  unsigned int (*write)(unsigned char *buf, unsigned int offset, unsigned int size);
  unsigned int (*erase)(unsigned int offset, unsigned int size);
};

struct flash_ops *get_flash(void);


#endif /*__CRC32_COMMAND_H__*/

