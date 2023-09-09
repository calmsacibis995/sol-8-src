#!/sbin/sh
#	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ident	"@(#)lastlogin.sh	1.7	99/03/08 SMI"	/* SVr4.0 1.6	*/
#	"lastlogin - keep record of date each person last logged in"
#	"bug - the date shown is usually 1 more than it should be "
#	"       because lastlogin is run at 4am and checks the last"
#	"       24 hrs worth of process accounting info (in pacct)"
PATH=/usr/lib/acct:/usr/bin:/usr/sbin
cd /var/adm/acct
if test ! -r sum/loginlog; then
	nulladm sum/loginlog
fi
#	"cleanup loginlog - delete entries of those no longer in"
#	"/etc/passwd and add an entry for those recently added"
#	"line 1 - get file of current logins in same form as loginlog"
#	"line 2 - merge the 2 files; use uniq to delete common"
#	"lines resulting in those lines which need to be"
#	"deleted or added from loginlog"
#	"line 3 - result of sort will be a file with 2 copies"
#	"of lines to delete and 1 copy of lines that are "
#	"valid; use uniq to remove duplicate lines"
getent passwd | sed "s/\([^:]*\).*/00-00-00  \1/" |\
sort +1 - sum/loginlog | uniq -u +10 |\
sort +1 - sum/loginlog |uniq -u > sum/tmploginlog
cp sum/tmploginlog sum/loginlog
#	"update loginlog"
_d="`date +%y-%m-%d`"
_day=`date +%m%d`
#	"lines 1 and 2 - remove everything from the total"
#	"acctng records with connect info except login"
#	"name and adds the date"
#	"line 3 - sorts in reverse order by login name; gets"
#	"1st occurrence of each login name and resorts by date"
acctmerg -a < nite/ctacct.$_day | \
sed -e "s/^[^ 	]*[ 	]\([^ 	]*\)[ 	].*/$_d  \1/" | \
nawk	'/^00-00-00/ {
                $0 = "00" $0
        }
	/^[0-9][0-9]-/ {
                d=substr($0,1,2);
                if (d<=68) {
                        $0 = "20" $0
                } else {
                        $0 = "19" $0
                }
        }
        { print }' - sum/loginlog | \
sort -r +1 | uniq +10 | sort | \
nawk	'/^[0-9][0-9][0-9][0-9]-/ {
	$0 = substr($0,3)
    }
    { print }' > sum/tmploginlog
cp sum/tmploginlog sum/loginlog
rm -f sum/tmploginlog