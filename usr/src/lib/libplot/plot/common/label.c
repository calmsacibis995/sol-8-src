/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright (c) 1997 by Sun Microsystems, Inc.
 * All rights reserved
 */

#pragma ident	"@(#)label.c	1.7	97/10/22 SMI"	/* SVr4.0 1.2	*/
/*LINTLIBRARY*/

#include <stdio.h>

void
label(char *s)
{
	int i;
	(void) putc('t', stdout);
	for (i = 0; s[i]; )
		(void) putc(s[i++], stdout);
	(void) putc('\n', stdout);
}
