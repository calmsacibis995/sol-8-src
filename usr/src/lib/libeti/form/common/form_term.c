/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 *      Copyright (c) 1997, by Sun Microsystems, Inc.
 *      All rights reserved.
 */

#pragma ident	"@(#)form_term.c	1.4	97/09/08 SMI" /* SVr4.0 1.1 */

/*LINTLIBRARY*/

#include <sys/types.h>
#include "utility.h"

	/*
	 *  set_form_term
	*/

int
set_form_term(FORM *f, PTF_void func)
{
	Form(f)->formterm = func;
	return (E_OK);
}

PTF_void
form_term(FORM *f)
{
	return (Form(f)->formterm);
}
