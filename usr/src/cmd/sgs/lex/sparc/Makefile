#
#ident	"@(#)Makefile	1.6	97/12/08 SMI"
#
# Copyright (c) 1993 by Sun Microsystems, Inc.
#
# cmd/sgs/lex/sparc/Makefile
#

PROG=		lex
INCLIST_sparc=	-I../common

include 	../../../Makefile.cmd
include		../Makefile.com

.KEEP_STATE:

all:		$(PROG)

all_lib:	$(LIBS)

install:	all $(ROOTCCSBINPROG) $(ROOTFORMS)

# The static library libl.a only gets installed for 32-bit, and is installed
# in /usr/ccs/lib rather than /usr/lib, hence the separate rule. In addition,
# we need to link /usr/ccs/lib/libl.so with /usr/lib/libl.so.1 to accomodate
# the order search of the linker
#
install_lib:	all_lib $(STATICLIB) $(ROOTLIBS) $(ROOTLINKS) $(DYNLINKLIB) $(ROOTLINT)

include		../Makefile.targ
include		../../../Makefile.targ
