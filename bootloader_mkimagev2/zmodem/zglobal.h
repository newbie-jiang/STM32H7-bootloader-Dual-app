#ifndef ZMODEM_GLOBAL_H
#define ZMODEM_GLOBAL_H

/* zglobal.h - prototypes etcetera for lrzsz

  Copyright (C) until 1998 Chuck Forsberg (OMEN Technology Inc)
  Copyright (C) 1994 Matt Porter
  Copyright (C) 1996, 1997 Uwe Ohse
  Copyright (C) 2018 Michael L. Gran

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
  02111-1307, USA.
*/

#include "config.h"

#include "zreadline.h"

//#define time_t int

#define OK 0
#define FALSE 0
#define TRUE 1
#define ERROR (-1)

/* Ward Christensen / CP/M parameters - Don't change these! */
#define ENQ 005
#define CAN ('X'&037)
#define XOFF ('s'&037)
#define XON ('q'&037)
#define SOH 1
#define STX 2
#define EOT 4
#define ACK 6
#define NAK 025
#define CPMEOF 032
#define WANTCRC 0103    /* send C not NAK to get crc not checksum */
#define WANTG 0107  /* Send G not NAK to get nonstop batch xmsn */
#define TIMEOUT (-2)
#define RCDO (-3)
#define WCEOT (-10)

#define RETRYMAX 10

#define UNIXFILE 0xF000  /* The S_IFMT file mask bit for stat */

#define DEFBYTL 2000000000L	/* default rx file size */

enum zm_type_enum {
	ZM_ZMODEM
};

struct zm_fileinfo {
	char *fname;
	int modtime;
	int mode;
	size_t bytes_total;
	size_t bytes_sent;
	size_t bytes_received;
	size_t bytes_skipped; /* crash recovery */
	int    eof_seen;
};

#define R_BYTESLEFT(x) ((x)->bytes_total-(x)->bytes_received)

extern const char *program_name;        /* the name by which we were called */
// extern int Verbose;
// extern int errors;
// extern int no_timeout;
// extern int under_rsh;


/* rbsb.c */
extern int Fromcu;
extern int iofd;

/* rbsb.c */
int rdchk (int fd);
int io_mode (int fd, int n);
void sendbrk (int fd);


/* zm.c */

#endif
