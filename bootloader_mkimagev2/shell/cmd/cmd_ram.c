#include <stdio.h>
#include <string.h>
#include "command.h"
#include "interface_cmd.h"





struct command ram_cmd = {
    "setenv",
    "Setting environment variables\r\n",
    "Usage: sevent <parameter> <value>\r\n",
    interface_ram,
};





