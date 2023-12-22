/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 */



#include "command.h"
#include "uart.h"
#include "uart.h"
#include "stm32h7xx.h"

static int reboot(int argc, char **argv)
{
  NVIC_SystemReset();     // ¸´Î»
}

struct command reboot_cmd = {
	"reboot",
		
	"software reset\r\n",
	
	"Usage: \r\n"
	"       reboot, will software reset\r\n",

	reboot,
};















