#
# ident	"@(#)Makefile	1.9	99/03/30 SMI"
#
# Copyright (c) 1989,1998 by Sun Microsystems, Inc.
# All rights reserved.
#
# cmd/lp/terminfo/Makefile
#

include 		../Makefile.lp

ROOTTERMINFO =		$(ROOT)/usr/share/lib/terminfo

TIC =			tic

SRCS =			PS.ti 40.ti 477.ti 53x0.ti 593.ti daisy.ti hplaser.ti \
			44x.ti 47x.ti 57x.ti dec.ti ibm.ti 45x.ti 495.ti \
			58x.ti citoh.ti epson.ti unknown.ti


TMPSRC =		terminfo.src

DIRMODE=		755
FILEMODE=		644
GROUP=			bin

.KEEP_STATE:

all :			$(TMPSRC) 

$(TMPSRC) :		$(SRCS)
			$(RM) $@; cat $(SRCS) > $@

#
#	Since all entries are created at once, we simply choose one of the
#	target files and assume everything will be made at one time. This
#	has holes (like if somebody removes P/PSR but not P/PS), but those
#	are the breaks.
#
install :		all $(ROOTTERMINFO) $(ROOTTERMINFO)/P/PS

$(ROOTTERMINFO)/P/PS:	$(TMPSRC)
	TERMINFO=$(ROOTTERMINFO) 2>&1 $(TIC) -v $(TMPSRC) > errs
	$(CHOWN) -R $(OWNER) $(ROOTTERMINFO)
	$(CHGRP) -R $(GROUP) $(ROOTTERMINFO)
	@$(ECHO) "\n`2>/dev/null cat errs|wc -l` entries have been compiled\n"
	@-(     2>/dev/null cat errs|grep -iv "^mkdir"|grep -iv "^create"|grep -iv "^link"|grep -vi $(TMPSRC)|grep -vi touch|grep -vi "working"; \
		if [ $$? -ne 0 ] ; \
		then \
			$(ECHO) "\tNo errors\n"; \
		else \
			$(ECHO) "\n\tErrors can be found in `pwd`/errs\n"; \
		fi \
	)

$(ROOTTERMINFO) :
			$(INS.dir)

clean clobber:
			$(RM) $(TMPSRC)

strip lint :

