/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 */



#include "command.h"
#include "uart.h"

static int go_f(int argc, char **argv)
{
#if 0	
	typedef void (*f_type)(void);
	f_type f;
	f = (f_type)0x20000000;
	f();
#else
//	start_app(0x20000000);
#endif
}

struct command go_cmd = {
	"go",
		
	"jump to 0x20000000 to rum app\r\n",
	
	"Usage: \r\n"
	"       go, jump to 0x20000000 to rum app\r\n",

	go_f,
};















