#include "ram_cmd.h"
#include "uart.h"
#include "stdio.h"




static int ram_function(int argc, char **argv) {
  

    
    return 0;  // ·µ»Ø³É¹¦Âë
}

struct command ram_cmd = {
    "setenv",
    "Setting environment variables\r\n",
    "Usage: sevent <parameter> <value>\r\n",
    ram_function,
};





