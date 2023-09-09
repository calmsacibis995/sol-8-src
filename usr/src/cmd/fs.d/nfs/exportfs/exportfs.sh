#!/sbin/sh
#	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T
#	  All Rights Reserved

#	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T
#	The copyright notice above does not evidence any
#	actual or intended publication of such source code.

#ident	"@(#)exportfs.sh	1.6	98/05/29 SMI"
#!/bin/sh
#
#  exportfs: compatibility script for SunOs command.  
#

USAGE="Usage: exportfs [-aviu] [-o options] directory"
DFSTAB=/etc/dfs/dfstab
OPTS="rw"

#
# Translate from exportfs opts to share opts
#

fixopts() {
	IFS=, ; set - $OPTS ; IFS=" "
	for i
		do case $i in *access=* ) eval $i ;; esac ; done
	if [ ! "$access" ] ; then return ; fi

	OPTS=""
	for i
	do
		case $i in
		rw=*     ) OPTS="$OPTS$i," ;;
		ro | rw  ) OPTS="${OPTS}$i=$access," ; ropt="true" ;;
		access=* ) ;;
		*        ) OPTS="$OPTS$i," ;;
		esac
	done
	if [ ! "$ropt" ] ; then OPTS="ro=$access,$OPTS" ; fi
	OPTS=`echo $OPTS | sed 's/,$//'`
}

bad() {
	echo $USAGE >&2
	exit 1
}

PATH=/usr/sbin:/usr/bin:$PATH
export PATH

if set -- `getopt aviuo: $*` ; then : ; else bad ; fi

for i in $*
do
	case $i in
	-a ) aflg="true" ; shift ;;	# share all nfs
	-v ) vflg="true" ; shift ;;	# verbose
	-i ) iflg="true" ; shift ;;	# ignore dfstab opts
	-u ) uflg="true" ; shift ;;	# unshare
	-o ) oflg="true" ; OPTS=$2 ; shift 2 ;;	# option string
	-- ) shift ; break ;;
	esac
done

if [ $aflg ] ; then
	if [ "$DIR" -o "$iflg" -o "$oflg"  ] ; then bad ; fi
	if [ $uflg ] ; then
		if [ $vflg ] ; then echo unshareall -F nfs ; fi
		/usr/sbin/unshareall -F nfs
	else
		if [ $vflg ] ; then echo shareall -F nfs ; fi
		/usr/sbin/shareall -F nfs
	fi
	exit $?
fi

case $# in
	0 ) if [ "$iflg" -o "$uflg" -o "$oflg" ] ; then bad ; fi
	    if [ "$vflg" ] ; then echo share -F nfs ; fi
	    /usr/sbin/share -F nfs
	    exit $? ;;

	1 ) DIR=$1 ;;
	* ) bad ;;
esac

if [ $uflg ] ; then
	if [ "$iflg" -o "$oflg" ] ; then bad ; fi
	if [ $vflg ] ; then echo unshare -F nfs $DIR ; fi
	/usr/sbin/unshare -F nfs $DIR
	exit $?
fi

if [ $iflg ] ; then
	fixopts
	if [ $vflg ] ; then echo share -F nfs -o $OPTS $DIR ; fi
	/usr/sbin/share -F nfs -o $OPTS $DIR
else
	CMD=`grep $DIR'[ 	]*$' $DFSTAB`
	if [ "$CMD" = "" ] ; then
		echo "exportfs: no entry for $DIR in $DFSTAB" >&2
		exit 1
	fi
	if [ $oflg ] ; then
		echo "exportfs: supplied options ignored" >&2
		vflg="true"
	fi
	if [ $vflg ] ; then echo $CMD ; fi
	eval $CMD
fi
exit $?
