#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "zmodem.h"
#include "command.h"


static unsigned int g_bytes;
static unsigned int g_download_address = 0x20000000;

unsigned int get_download_address(void)
{
	return g_download_address;
}

unsigned int get_download_size(void)
{
	return g_bytes;
}


/* rz 0x20000000 */
int
rz(int argc, char *argv[])
{
//  int c;
  bool bps_flag = false;
  uint64_t bps = 0u;

  if (argc == 2)
  	g_download_address = str2hex(argv[1]);

  g_bytes = zmodem_receive(NULL, /* use current directory */
				NULL, /* receive everything */
				NULL,
				NULL,
				bps_flag ? bps : 0,
				RZSZ_FLAGS_NONE);
  //fprintf(stderr, "Received %zu bytes.\n", bytes);
  putstr("Received ");
  puthex(g_bytes);
  putstr("bytes.\r\n");
  
  putstr("Saved at ram address ");
  puthex(g_download_address);
  putstr("\r\n");
  return 0;
}

struct command rz_cmd = {
	"rz",
		
	"receive file through z modem\r\n",
	
	"Usage: \r\n"
	"       rz [mem address]\r\n",

	rz,
};




