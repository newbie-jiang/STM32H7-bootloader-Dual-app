/*
  zreadline.c - line reading stuff for lrzsz
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

  originally written by Chuck Forsberg
*/
/* once part of lrz.c, taken out to be useful to lsz.c too */

#include "zglobal.h"

#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
//#include "bsp_usart.h" 
#include "log.h"
//#include "bsp_usart.h"

/* Ward Christensen / CP/M parameters - Don't change these! */
#define TIMEOUT (-2)


static int readline_internal(zreadline_t *zr, unsigned int timeout);

static zreadline_t g_zr;


zreadline_t * zreadline_init(int fd, size_t readnum, size_t bufsize, int no_timeout)
{
	zreadline_t *zr = &g_zr; // (zreadline_t *) malloc (sizeof(zreadline_t));
	memset (zr, 0, sizeof(zreadline_t));
	zr->readline_fd = fd;
	zr->readline_readnum = readnum;
#if 0	
	zr->readline_buffer = malloc(bufsize > readnum ? bufsize : readnum);
	if (!zr->readline_buffer) {
		log_fatal(_("out of memory"));
		exit(1);
	}
#endif	
	zr->no_timeout = no_timeout;
	return zr;
}

int
zreadline_getc(zreadline_t *zr, int timeout)
{
#if 0	
	zr->readline_left --;
	if (zr->readline_left >= 0) {
		char c = *(zr->readline_ptr);
		zr->readline_ptr ++;
		return (unsigned char) c;
	}
	else
#endif		
		return readline_internal(zr, timeout);
}


/*
 * This version of readline is reasonably well suited for
 * reading many characters.
 *
 * timeout is in tenths of seconds
 */

static int readline_internal(zreadline_t *zr, unsigned int timeout)
{
//	return TIMEOUT;
	char c = my_getchar();
	return (unsigned char) c;
}



void zreadline_flush(zreadline_t *zr)
{
	zr->readline_left=0;
//	return;
}


void zreadline_flushline(zreadline_t *zr)
{
  //zr->readline_left = 0;
  //lseek(zr->readline_fd, 0, SEEK_END);
  
}

/* send cancel string to get the other end to shut up */

void zreadline_canit (zreadline_t *zr, int fd)
{
	static char canistr[] =
	{
		24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 0
	};
	zreadline_flushline(zr);
	//write(fd,canistr,strlen(canistr));
	putdatas(canistr,strlen(canistr));
	//if (fd==0)
	//	write(1,canistr,strlen(canistr));
}



