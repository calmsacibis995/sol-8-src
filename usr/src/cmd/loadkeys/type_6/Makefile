#
# @(#)Makefile 1.1 99/02/25 SMI
#
#
# Copyright (c) 1999 by Sun Microsystems, Inc.
# All rights reserved.
#
# cmd/loadkeys/type_6/Makefile

KEYTABLES = \
	us denmark finnish france germany italy japan korea \
	netherlands norway portugal reset spain sweden \
        swiss_french swiss_german taiwan uk

include ../../Makefile.cmd

ROOTKEYTABLESDIR= $(ROOTSHLIB)/keytables
ROOTKEYDIR= $(ROOTKEYTABLESDIR)/type_6
ROOTKEYTABLES= $(KEYTABLES:%=$(ROOTKEYDIR)/%)

# there is an install target for each ROOT layout link
#
LAYOUTS= \
	layout_00 layout_06 layout_07 layout_08 layout_09 layout_0e \
        layout_0f layout_10 layout_12 layout_13 layout_16 layout_19 \
        layout_1a layout_1b layout_1c layout_1e layout_20 layout_21

ROOTLINKS= $(LAYOUTS:%=$(ROOTKEYDIR)/%)

$(ROOTKEYTABLES) := FILEMODE = 444

# install rule
$(ROOTKEYDIR)/%: %
	$(INS.file)

.KEEP_STATE:

all:  $(KEYTABLES)

install: all $(ROOTKEYTABLESDIR) $(ROOTKEYDIR) $(ROOTKEYTABLES) $(ROOTLINKS)

clean:

$(ROOTKEYTABLESDIR):
	$(INS.dir)

$(ROOTKEYDIR): $(ROOTKEYTABLESDIR)
	$(INS.dir)

# install targets for ROOT layout links
#
$(ROOTKEYDIR)/layout_00: $(ROOTKEYDIR)/us
	$(RM) $@; $(LN) $(ROOTKEYDIR)/us $@

$(ROOTKEYDIR)/layout_06: $(ROOTKEYDIR)/denmark
	$(RM) $@; $(LN) $(ROOTKEYDIR)/denmark $@

$(ROOTKEYDIR)/layout_07: $(ROOTKEYDIR)/finnish
	$(RM) $@; $(LN) $(ROOTKEYDIR)/finnish $@

$(ROOTKEYDIR)/layout_08: $(ROOTKEYDIR)/france
	$(RM) $@; $(LN) $(ROOTKEYDIR)/france $@

$(ROOTKEYDIR)/layout_09: $(ROOTKEYDIR)/germany
	$(RM) $@; $(LN) $(ROOTKEYDIR)/germany $@

$(ROOTKEYDIR)/layout_0e: $(ROOTKEYDIR)/italy
	$(RM) $@; $(LN) $(ROOTKEYDIR)/italy $@

$(ROOTKEYDIR)/layout_0f: $(ROOTKEYDIR)/japan
	$(RM) $@; $(LN) $(ROOTKEYDIR)/japan $@

$(ROOTKEYDIR)/layout_10: $(ROOTKEYDIR)/korea
	$(RM) $@; $(LN) $(ROOTKEYDIR)/korea $@

$(ROOTKEYDIR)/layout_12: $(ROOTKEYDIR)/netherlands
	$(RM) $@; $(LN) $(ROOTKEYDIR)/netherlands $@

$(ROOTKEYDIR)/layout_13: $(ROOTKEYDIR)/norway
	$(RM) $@; $(LN) $(ROOTKEYDIR)/norway $@

$(ROOTKEYDIR)/layout_16: $(ROOTKEYDIR)/portugal
	$(RM) $@; $(LN) $(ROOTKEYDIR)/portugal $@

$(ROOTKEYDIR)/layout_19: $(ROOTKEYDIR)/spain
	$(RM) $@; $(LN) $(ROOTKEYDIR)/spain $@

$(ROOTKEYDIR)/layout_1a: $(ROOTKEYDIR)/sweden
	$(RM) $@; $(LN) $(ROOTKEYDIR)/sweden $@

$(ROOTKEYDIR)/layout_1b: $(ROOTKEYDIR)/swiss_french
	$(RM) $@; $(LN) $(ROOTKEYDIR)/swiss_french $@

$(ROOTKEYDIR)/layout_1c: $(ROOTKEYDIR)/swiss_german
	$(RM) $@; $(LN) $(ROOTKEYDIR)/swiss_german $@

$(ROOTKEYDIR)/layout_1e: $(ROOTKEYDIR)/taiwan
	$(RM) $@; $(LN) $(ROOTKEYDIR)/taiwan $@

$(ROOTKEYDIR)/layout_20: $(ROOTKEYDIR)/uk
	$(RM) $@; $(LN) $(ROOTKEYDIR)/uk $@

$(ROOTKEYDIR)/layout_21: $(ROOTKEYDIR)/us
	$(RM) $@; $(LN) $(ROOTKEYDIR)/us $@

include ../../Makefile.targ
