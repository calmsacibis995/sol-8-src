#
# Copyright (c) 1997-1999 by Sun Microsystems, Inc.
# All rights reserved.
#
#ident	"@(#)Makefile	1.17	99/01/25 SMI"
#
# lib/nametoaddr/straddr/Makefile

include		../../Makefile.lib

SUBDIRS=	spec .WAIT $(MACH) $(BUILD64) $(MACH64)

all :=		TARGET= all
clean :=	TARGET= clean
clobber :=	TARGET= clobber
delete :=	TARGET= delete
install :=	TARGET= install
lint :=		TARGET= lint
catalog :=	TARGET= catalog
package :=	TARGET= package

.KEEP_STATE:

all clean clobber delete install lint catalog package: $(SUBDIRS)

$(MACH) $(MACH64) spec:	FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:
