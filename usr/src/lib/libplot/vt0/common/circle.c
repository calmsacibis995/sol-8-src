/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright (c) 1997, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma ident	"@(#)circle.c	1.7	97/10/02 SMI"	/* SVr4.0 1.2	*/
/*LINTLIBRARY*/

#include <unistd.h>
#include "con.h"

/*ARGSUSED*/
void
circle(short x, short y, short r)
{
	char c = 5;
	(void) write(vti, &c, 1);
	(void) write(vti, &x, 6);
}
