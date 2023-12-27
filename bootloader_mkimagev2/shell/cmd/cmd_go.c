

#include <stdio.h>
#include <string.h>
#include "command.h"
#include "interface_cmd.h"


struct command go_cmd = {
	"go",
		
	"jump to 0x20000000 to rum app\r\n",
	
	"Usage: \r\n"
	"       go, jump to 0x20000000 to rum app\r\n",

	interface_go,
};















