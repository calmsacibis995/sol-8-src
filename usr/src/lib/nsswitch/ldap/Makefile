#
#ident  "@(#)Makefile 1.1     99/07/07 SMI"
#
# Copyright (c) 1999 by Sun Microsystems, Inc.
# All rights reserved.
#
# lib/nsswitch/ldap/Makefile
#

include     ../../../Makefile.master

FILES_SUBDIRS=    $(MACH) $(BUILD64) $(MACH64)

all:=		TARGET= all
clean:=		TARGET= clean
clobber:=	TARGET= clobber
install:=	TARGET= install
lint:=		TARGET= lint

.KEEP_STATE:

all clean clobber install lint: $(FILES_SUBDIRS)

$(MACH) $(MACH64): FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

FRC:
