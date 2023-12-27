#include <stdio.h>
#include <string.h>
#include "command.h"
#include "interface_cmd.h"



struct command reboot_cmd = {
	"reboot",
		
	"software reset\r\n",
	
	"Usage: \r\n"
	"       reboot, will software reset\r\n",

	interface_reboot,
};















