/*
  rbsb.c - terminal handling stuff for lrzsz
  Copyright (C) until 1988 Chuck Forsberg (Omen Technology INC)
  Copyright (C) 1994 Matt Porter, Michael D. Black
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

/*
 *  Rev 05-05-1988
 *  ============== (not quite, but originated there :-). -- uwe
 */
#include "zglobal.h"

#include <stdio.h>
#include <errno.h>
#include "log.h"



/*
 * mode(n)
 *  3: save old tty stat, set raw mode with flow control
 *  2: set XON/XOFF for sb/sz with ZMODEM
 *  1: save old tty stat, set raw mode
 *  0: restore original tty mode
 * Returns the output baudrate, or zero on failure
 */

int io_mode(int fd, int n)
{
	return 115200;
}


void sendbrk(int fd)
{
	//tcsendbreak(fd,0);
}


/* End of rbsb.c */
