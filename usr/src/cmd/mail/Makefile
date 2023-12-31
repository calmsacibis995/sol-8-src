#
#ident	"@(#)Makefile	1.25	99/03/09 SMI"
#
# Copyright (c) 1989 by Sun Microsystems, Inc.
#
# cmd/mail/Makefile
#


BINPROG=	mail 

PROG= 	       $(BINPROG)

GREP=		grep

include ../Makefile.cmd

CLOBBERFILES=	$(PROG)
CLEANFILES=	$(MAILOBJS)

MAILOBJS= 	add_recip.o cat.o ckdlivopts.o \
		cksaved.o clr_hinfo.o copyback.o copylet.o \
		copymt.o createmf.o del_recipl.o Dout.o  delete.o \
		done.o doopen.o dumpaff.o dumprcv.o \
		errmsg.o gendeliv.o getarg.o getcomment.o \
		gethead.o goback.o init.o \
		isheader.o isit.o islocal.o istext.o legal.o \
		lock.o  main.o mkdead.o mta_ercode.o \
		new_recipl.o parse.o pckaffspot.o pckrcvspot.o \
		pickFrom.o pipletr.o poplist.o printhdr.o printmail.o \
		pushlist.o savehdrs.o sel_disp.o  sendlist.o \
		sendmail.o  setsig.o \
		stamp.o Tout.o maid.o

MAILSRC=	 $(MAILOBJS:%.o=%.c)

CPROG=		mail
CPROGSRC= 	$(CPROG:%=_%.c)

# ROOT directories
ROOTMAILD=	$(ROOTLIB)/mail
ROOTINC=	$(ROOT)/usr/include

# installed modules
ROOTBINPROG=	$(BINPROG:%=$(ROOTBIN)/%)

ROOTSYMLINKS=	$(ROOTBIN)/rmail


EDITPATH= \
	sed -e 's!REAL_PATH!/usr/bin!g' \
	    -e 's!USR_SHARE_LIB!/usr/share/lib!g' \
	    -e 's!VAR_MAIL!/var/mail!g' < $? > $@

CPPFLAGS =	-DSVR4 -I. $(CPPFLAGS.master)

LDLIBS +=	-lmail -lsocket -lnsl

# conditional assignments
#
all:=		TARGET= all
install:=	TARGET= install
clean:=		TARGET= clean
clobber:=	TARGET= clobber
lint:=		TARGET= lint

# file modes, owners and groups for the install target
#


$(ROOTBIN)/mail :=	FILEMODE =	02511
$(ROOTBIN)/mail :=	GROUP =	mail


# install rules
$(ROOTINC)/% : %
	$(INS.file)

.KEEP_STATE:

.PARALLEL: $(MAILOBJS) $(OBJS)

all:		$(PROG) 

mail: 		$(MAILOBJS)
	$(LINK.c) -o $@ $(MAILOBJS) $(LDLIBS)
	$(POST_PROCESS)

install: all .WAIT $(ROOTBINPROG) $(ROOTSYMLINKS)

# ROOTSYMLINKS
#
$(ROOTBIN)/rmail:
	$(RM) $@; $(SYMLINK) mail $@

lint: 
	$(LINT.c) $(MAILSRC)

clean:
	$(RM) $(OBJS) $(MAILOBJS)

clobber:
	$(RM) $(OBJS) $(MAILOBJS) mail
