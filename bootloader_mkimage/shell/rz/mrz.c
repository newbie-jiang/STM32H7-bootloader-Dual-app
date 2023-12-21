#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zmodem.h"
#include "command.h"


int
rz(int argc, char *argv[])
{
//  int c;
  bool bps_flag = false;
  uint64_t bps = 0u;

  size_t bytes = zmodem_receive(NULL, /* use current directory */
				NULL, /* receive everything */
				NULL,
				NULL,
				bps_flag ? bps : 0,
				RZSZ_FLAGS_NONE);
  //fprintf(stderr, "Received %zu bytes.\n", bytes);
  return 0;
}

struct command rz_cmd = {
	"rz",
		
	"receive file through z modem\r\n",
	
	"Usage: \r\n"
	"       rz <mem address>\r\n",

	rz,
};




