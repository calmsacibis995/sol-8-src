#!/sbin/sh
#	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ident	"@(#)cxref.sh	1.5	92/07/14 SMI"	/* SVr4.0 1.4	*/
grep -n "^[abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ]" $* > /tmp/$$
ex - /tmp/$$ <<\!
v/(.*)$/d
g/STATIC/d
g/\<static\>/d
g/\<long\>/d
g/\<short\>/d
g/\<line\>/d
g/\<switch\>/d
g/\<unsigned\>/d
g/\<return\>/d
g/\<break\>/d
g/\<bool\>/d
g/\<boolean\>/d
g/\<case\>/d
g/\<struct\>/d
g/\<int\>/d
g/\<char\>/d
g/\<extern\>/d
g/:$/d
g/\\/d
1,$s/\(.*:\)\(.*\)/\2|\1/
1,$s/|/                                                 /
1,$s/^\(................................................\) */\1/
w
q
!
sort /tmp/$$
rm /tmp/$$