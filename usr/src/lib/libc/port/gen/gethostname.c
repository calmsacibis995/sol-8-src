/*
 *	Copyright (c) 19986-1996, by Sun Microsystems, Inc.
 *	All rights reserved.
 */

/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *		PROPRIETARY NOTICE (Combined)
 *
 * This source code is unpublished proprietary information
 * constituting, or derived under license from AT&T's UNIX(r) System V.
 * In addition, portions of such source code were derived from Berkeley
 * 4.3 BSD under license from the Regents of the University of
 * California.
 */

#pragma	ident	"@(#)gethostname.c	1.2	96/11/14 SMI"	/* SVr4.0 1.3	*/

/*LINTLIBRARY*/

#include "synonyms.h"
#include <sys/types.h>
#include <sys/systeminfo.h>

int
gethostname(char *name, int namelen)
{
	return (sysinfo(SI_HOSTNAME, name, namelen) == -1 ? -1 : 0);
}

int
sethostname(char *name, int namelen)
{
	return (sysinfo(SI_SET_HOSTNAME, name, namelen) == -1 ? -1 : 0);
}
