#ident	"@(#)Makefile	1.1	99/10/08 SMI"
#
# Generated automatically from Makefile.in by configure.
## -*- text -*- #############################################################
#								            #
# Makefile for the Bash versions of the GNU Readline and History Libraries. #
#								            #
#############################################################################

srcdir = .
VPATH = .:.
topdir = ../..
BUILD_DIR = ../..

include ../../../Makefile.cmd

SHELL = /bin/sh

# Programs to make tags files.
ETAGS = etags -tw
CTAGS = ctags -tw

# I hate to do this, but it _isn't_ our code.
LOCAL_CFLAGS = -errtags=yes -erroff=E_END_OF_LOOP_CODE_NOT_REACHED \
    -erroff=E_EMPTY_TRANSLATION_UNIT -erroff=E_STATEMENT_NOT_REACHED \
    -erroff=E_NON_CONST_INIT

CFLAGS += -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
LDFLAGS =  

DEFS = -DHAVE_CONFIG_H
LOCAL_DEFS = -DSHELL

INCLUDES = -I. -I$(BUILD_DIR) -I$(topdir) -I$(topdir)/lib

CFLAGS += $(DEFS) $(LOCAL_DEFS) $(APP_CFLAGS) ${INCLUDES} \
    $(LOCAL_CFLAGS)

.KEEP_STATE:

# The name of the main library target.
LIBRARY_NAME = libreadline.a

# The C code source files for this library.
CSOURCES = $(srcdir)/readline.c $(srcdir)/funmap.c $(srcdir)/keymaps.c \
	   $(srcdir)/vi_mode.c $(srcdir)/parens.c $(srcdir)/rltty.c \
	   $(srcdir)/complete.c $(srcdir)/bind.c $(srcdir)/isearch.c \
	   $(srcdir)/display.c $(srcdir)/signals.c $(srcdir)/emacs_keymap.c \
	   $(srcdir)/vi_keymap.c $(srcdir)/util.c $(srcdir)/kill.c \
	   $(srcdir)/undo.c $(srcdir)/macro.c $(srcdir)/input.c \
	   $(srcdir)/callback.c $(srcdir)/terminal.c $(srcdir)/xmalloc.c \
	   $(srcdir)/history.c $(srcdir)/histsearch.c $(srcdir)/histexpand.c \
	   $(srcdir)/histfile.c $(srcdir)/nls.c $(srcdir)/search.c \
	   $(srcdir)/shell.c $(srcdir)/tilde.c $(srcdir)/savestring.c

# The header files for this library.
HSOURCES = readline.h rldefs.h chardefs.h keymaps.h history.h histlib.h \
	   posixstat.h posixdir.h posixjmp.h tilde.h rlconf.h rltty.h \
	   ansi_stdlib.h rlstdc.h tcap.h

HISTOBJ = history.o histexpand.o histfile.o histsearch.o shell.o savestring.o
TILDEOBJ = tilde.o
OBJECTS = readline.o vi_mode.o funmap.o keymaps.o parens.o search.o \
	  rltty.o complete.o bind.o isearch.o display.o signals.o \
	  util.o kill.o undo.o macro.o input.o callback.o terminal.o \
	  nls.o $(HISTOBJ) $(TILDEOBJ) xmalloc.o

# The texinfo files which document this library.
DOCSOURCE = doc/rlman.texinfo doc/rltech.texinfo doc/rluser.texinfo
DOCOBJECT = doc/readline.dvi
DOCSUPPORT = doc/Makefile
DOCUMENTATION = $(DOCSOURCE) $(DOCOBJECT) $(DOCSUPPORT)

SUPPORT = Makefile ChangeLog $(DOCSUPPORT) examples/[-a-z.]*

SOURCES  = $(CSOURCES) $(HSOURCES) $(DOCSOURCE)

THINGS_TO_TAR = $(SOURCES) $(SUPPORT)

INSTALLED_HEADERS = readline.h chardefs.h keymaps.h history.h tilde.h

##########################################################################

all: libreadline.a libhistory.a

libreadline.a: $(OBJECTS)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

libhistory.a: $(HISTOBJ) xmalloc.o
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(HISTOBJ) xmalloc.o

documentation: force
	test -d doc || mkdir doc
	-( cd doc && $(MAKE) $(MFLAGS) )

force:

install:
	@echo "This version of the readline library should not be installed."

uninstall:
	@echo "This version of the readline library should not be installed."

TAGS:	force
	$(ETAGS) $(CSOURCES) $(HSOURCES)

tags:	force
	$(CTAGS) $(CSOURCES) $(HSOURCES)

clean:	force
	$(RM) $(OBJECTS) *.a
	-( cd doc && $(MAKE) $(MFLAGS) $@ )

mostlyclean: clean
	-( cd doc && $(MAKE) $(MFLAGS) $@ )

distclean maintainer-clean: clean
	-( cd doc && $(MAKE) $(MFLAGS) $@ )
	$(RM) Makefile
	$(RM) TAGS tags

# Dependencies
bind.o: ansi_stdlib.h posixstat.h
bind.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
bind.o: readline.h keymaps.h chardefs.h tilde.h
bind.o: history.h rlstdc.h
callback.o: rlconf.h
callback.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
callback.o: readline.h keymaps.h chardefs.h tilde.h rlstdc.h
complete.o: ansi_stdlib.h posixdir.h posixstat.h
complete.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
complete.o: readline.h keymaps.h chardefs.h tilde.h rlstdc.h
display.o: ansi_stdlib.h posixstat.h
display.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
display.o: tcap.h
display.o: readline.h keymaps.h chardefs.h tilde.h
display.o: history.h rlstdc.h
funmap.o: readline.h keymaps.h chardefs.h tilde.h
funmap.o: rlconf.h ansi_stdlib.h rlstdc.h
funmap.o: ${BUILD_DIR}/config.h
histexpand.o: ansi_stdlib.h
histexpand.o: history.h histlib.h rlstdc.h
histexpand.o: ${BUILD_DIR}/config.h
histfile.o: ansi_stdlib.h
histfile.o: history.h histlib.h rlstdc.h
histfile.o: ${BUILD_DIR}/config.h
history.o: ansi_stdlib.h
history.o: history.h histlib.h rlstdc.h
history.o: ${BUILD_DIR}/config.h
histsearch.o: ansi_stdlib.h
histsearch.o: history.h histlib.h rlstdc.h
histsearch.o: ${BUILD_DIR}/config.h
input.o: ansi_stdlib.h
input.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
input.o: readline.h keymaps.h chardefs.h tilde.h rlstdc.h
isearch.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
isearch.o: readline.h keymaps.h chardefs.h tilde.h
isearch.o: ansi_stdlib.h history.h rlstdc.h
keymaps.o: emacs_keymap.c vi_keymap.c
keymaps.o: keymaps.h chardefs.h rlconf.h ansi_stdlib.h
keymaps.o: readline.h keymaps.h chardefs.h tilde.h
keymaps.o: ${BUILD_DIR}/config.h rlstdc.h
kill.o: ansi_stdlib.h
kill.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
kill.o: readline.h keymaps.h chardefs.h tilde.h
kill.o: history.h rlstdc.h
macro.o: ansi_stdlib.h
macro.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
macro.o: readline.h keymaps.h chardefs.h tilde.h
macro.o: history.h rlstdc.h
nls.o: ansi_stdlib.h
nls.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
parens.o: rlconf.h
parens.o: ${BUILD_DIR}/config.h
parens.o: readline.h keymaps.h chardefs.h tilde.h rlstdc.h
readline.o: readline.h keymaps.h chardefs.h tilde.h
readline.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
readline.o: history.h rlstdc.h
readline.o: posixstat.h ansi_stdlib.h posixjmp.h
rltty.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
rltty.o: rltty.h
rltty.o: readline.h keymaps.h chardefs.h tilde.h rlstdc.h
search.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
search.o: readline.h keymaps.h chardefs.h tilde.h
search.o: ansi_stdlib.h history.h rlstdc.h
shell.o: ${BUILD_DIR}/config.h ansi_stdlib.h
signals.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
signals.o: readline.h keymaps.h chardefs.h tilde.h
signals.o: history.h rlstdc.h
terminal.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
terminal.o: tcap.h
terminal.o: readline.h keymaps.h chardefs.h tilde.h
terminal.o: history.h rlstdc.h
tilde.o: ansi_stdlib.h
tilde.o: ${BUILD_DIR}/config.h
tilde.o: tilde.h
undo.o: ansi_stdlib.h
undo.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
undo.o: readline.h keymaps.h chardefs.h tilde.h
undo.o: history.h rlstdc.h
util.o: posixjmp.h ansi_stdlib.h
util.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
util.o: readline.h keymaps.h chardefs.h tilde.h rlstdc.h
vi_mode.o: rldefs.h ${BUILD_DIR}/config.h rlconf.h
vi_mode.o: readline.h keymaps.h chardefs.h tilde.h
vi_mode.o: history.h ansi_stdlib.h rlstdc.h
xmalloc.o: ${BUILD_DIR}/config.h
xmalloc.o: ansi_stdlib.h

# Rules for deficient makes, like SunOS and Solaris
bind.o: bind.c
callback.o: callback.c
complete.o: complete.c
display.o: display.c
funmap.o: funmap.c
histexpand.o: histexpand.c
histfile.o: histfile.c
history.o: history.c
histsearch.o: histsearch.c
input.o: input.c
isearch.o: isearch.c
keymaps.o: keymaps.c emacs_keymap.c vi_keymap.c
kill.o: kill.c
macro.o: macro.c
nls.o: nls.c
parens.o: parens.c
readline.o: readline.c
rltty.o: rltty.c
savestring.o: savestring.c
search.o: search.c
shell.o: shell.c
signals.o: signals.c
terminal.o: terminal.c
tilde.o: tilde.c
undo.o: undo.c
util.o: util.c
vi_mode.o: vi_mode.c
xmalloc.o: xmalloc.c

include ../../../Makefile.targ
