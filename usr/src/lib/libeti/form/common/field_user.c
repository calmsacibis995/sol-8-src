/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *      Copyright (c) 1997, by Sun Microsystems, Inc.
 *      All rights reserved.
 */

#pragma ident	"@(#)field_user.c	1.5	97/09/17 SMI" /* SVr4.0 1.1 */

/*LINTLIBRARY*/

#include <sys/types.h>
#include "utility.h"

int
set_field_userptr(FIELD *f, char *userptr)
{
	Field(f)-> usrptr = userptr;
	return (E_OK);
}

char *
field_userptr(FIELD *f)
{
	return (Field(f)->usrptr);
}
