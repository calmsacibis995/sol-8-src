#
# uts/sun4u/javelin/Makefile
# Copyright (c) 1994-1998 by Sun Microsystems, Inc.
# All rights reserved.
#
#ident	"@(#)Makefile	1.5	99/08/24 SMI"
#
#	This makefile drives the production of all Javelin system
#	dependent modules for the sun4u architecture.
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE = ../..

#
#	Include common rules.
#
include $(UTSBASE)/sun4u/javelin/Makefile.javelin

def		:=	TARGET= def
all		:=	TARGET= all
install		:=	TARGET= install
install_h	:=	TARGET= install_h
clean		:=	TARGET= clean
clobber		:=	TARGET= clobber
lint		:=	TARGET= lint
lintlib		:=	TARGET= lintlib
modlintlib	:=	TARGET= modlintlib
clean.lint	:=	TARGET= clean.lint
check		:=	TARGET= check

#
#	Default build targets.
#
.KEEP_STATE:

def all clean.lint clean clobber: $(JAVELIN_KMODS)

modlintlib:	$(JAVELIN_KMODS)

install: 	$(ROOT_JAVELIN_DIR) $(USR_JAVELIN_DIR) \
		$(USR_JAVELIN_INC_DIR) \
		$(USR_JAVELIN_SBIN_DIR) \
		$(USR_JAVELIN_LIB_DIR) \
		.WAIT $(JAVELIN_KMODS)

install_h check:

lint:		modlintlib

#
# The 'lint.platmod' target lints the javelin platform module against the sun4u
# kernel.  This ends up doing all the kernel cross-checks, so it takes a couple
# of minutes.  Due to the low ROI, it's not run by default, but it's a good
# idea to run this if you change os/javelin.c.
#
LINT_LIBS	 = $(LINT_LIB) \
		   -L$(JAVELIN_LINT_LIB_DIR) \
		   -L$(LINT_LIB_DIR) $(LINT_KMODS:%=-l%) \
		   -L$(SPARC_LIB_DIR) $(SPARC_LINTS:%=-l%)

lint.platmod:   modlintlib
	@-$(ECHO) "\nJavelin Platform-dependent module: global crosschecks:"
	@-$(LINT) $(LINTFLAGS) $(LINT_LIBS) 2>&1 | $(LGREP.2)

$(JAVELIN_KMODS):	FRC
	@cd $@; pwd; $(MAKE) $(TARGET)

#
#	Include common targets.
#
include $(UTSBASE)/sun4u/javelin/Makefile.targ
