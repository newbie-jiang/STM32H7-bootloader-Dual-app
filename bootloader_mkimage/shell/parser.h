/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2011-06-02     Bernard      Add finsh_get_prompt function declaration
 */

#ifndef __PARSER_H__
#define __PARSER_H__

#include "types.h"

int msh_exec(char *cmd, rt_size_t length);


#endif

