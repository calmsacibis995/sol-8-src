/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)checkver.c	1.7	97/08/25 SMI"       /* SVr4.0 1.1*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <rpc/trace.h> 

# define VSTR	"# VERSION="


/*
 * check_version - check to make sure designated file is the correct version
 *		returns : 0 - version correct
 *			  1 - version incorrect
 *			  2 - could not open file
 *			  3 - corrupt file
 */


check_version(ver, fname)
int ver;
char *fname;
{
	FILE *fp;		/* file pointer for sactab */
	char line[BUFSIZ];	/* temp buffer for input */
	char *p;		/* working pointer */
	int version;		/* version number from sactab */

	trace2(TR_check_version, 0, ver);
	if ((fp = fopen(fname, "r")) == NULL) {
		trace2(TR_check_version, 1, ver);
		return(2);
	}
	p = line;
	while (fgets(p, BUFSIZ, fp)) {
		if (!strncmp(p, VSTR, strlen(VSTR))) {
			p += strlen(VSTR);
			if (*p)
				version = atoi(p);
			else {
				trace2(TR_check_version, 1, ver);
				return(3);
			}
			(void) fclose(fp);
			trace2(TR_check_version, 1, ver);
			return((version != ver) ? 1 : 0);
		}
		p = line;
	}
	trace2(TR_check_version, 1, ver);
	return(3);
}
