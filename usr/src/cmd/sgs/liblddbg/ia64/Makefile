#
#	Copyright (c) 1997,1999 by Sun Microsystems, Inc.
#	All rights reserved.
#
#pragma ident	"@(#)Makefile	1.1	99/05/04 SMI"
#


include		$(SRC)/cmd/sgs/liblddbg/Makefile.com

ROOTLIBDIR=	$(ROOT)/usr/lib/$(MACH64)
ROOTDYNLIB64=	$(DYNLIB:%=$(ROOTLIBDIR)/%)

.KEEP_STATE:

all:		$(DYNLIB) $(LIBLINKS)

install:	all $(ROOTDYNLIB64)

include		$(SRC)/cmd/sgs/liblddbg/Makefile.targ
include		../../Makefile.sub.64

CONVLIB	=	-L../../libconv/ia64

#LINTFLAGS64 +=	-D__sparcv9
