#ident	"@(#)Makefile	1.1	99/10/08 SMI"
#
# Generated automatically from Makefile.in by configure.
#
# Makefile for the Bash library
#

srcdir = .
VPATH = .:.
topdir = ../..
BUILD_DIR = ../..

include ../../../Makefile.cmd

POSIX_INC = ${topdir}/lib/posixheaders

SHELL = /bin/sh

# I hate to do this, but it _isn't_ our code.
LOCAL_CFLAGS = -errtags=yes -erroff=E_END_OF_LOOP_CODE_NOT_REACHED \
    -erroff=E_EMPTY_TRANSLATION_UNIT

CFLAGS += -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE64_SOURCE
LDFLAGS =   

PROFILE_FLAGS = 

DEFS = -DHAVE_CONFIG_H
LOCAL_DEFS = -DSHELL

INCLUDES = -I. -I../.. -I$(topdir) -I$(topdir)/lib -I$(POSIX_INC) -I$(srcdir)

CFLAGS += ${PROFILE_FLAGS} ${INCLUDES} $(DEFS) $(LOCAL_DEFS) $(LOCAL_CFLAGS)

.KEEP_STATE:

# The name of the library target.
LIBRARY_NAME = libsh.a

# The C code source files for this library.
CSOURCES = clktck.c getcwd.c getenv.c oslib.c setlinebuf.c \
	   strcasecmp.c strerror.c strtod.c strtol.c strtoul.c \
	   vprint.c itos.c rename.c

# The header files for this library.
HSOURCES = 

# The object files contained in $(LIBRARY_NAME)
OBJECTS = clktck.o getcwd.o getenv.o oslib.o setlinebuf.o \
	  strcasecmp.o strerror.o strtod.o strtol.o strtoul.o \
	  vprint.o itos.o rename.o

SUPPORT = Makefile

all: $(LIBRARY_NAME)

$(LIBRARY_NAME): $(OBJECTS)
	$(RM) $@
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

force:

# The rule for 'includes' is written funny so that the if statement
# always returns TRUE unless there really was an error installing the
# include files.
install:

clean:
	$(RM) $(OBJECTS) $(LIBRARY_NAME)

realclean distclean maintainer-clean: clean
	$(RM) Makefile

mostlyclean: clean

# Dependencies

# rules for losing makes, like SunOS
clktck.o: clktck.c
getcwd.o: getcwd.c
getenv.o: getenv.c
itos.o: itos.c
oslib.o: oslib.c
rename.o: rename.c
setlinebuf.o: setlinebuf.c
strcasecmp.o: strcasecmp.c
strerror.o: strerror.c
strtod.o: strtod.c
strtol.o: strtol.c
strtoul.o: strtoul.c
vprint.o: vprint.c

# all files in the library depend on config.h
clktck.o: ${BUILD_DIR}/config.h
getcwd.o: ${BUILD_DIR}/config.h
getenv.o: ${BUILD_DIR}/config.h
itos.o: ${BUILD_DIR}/config.h
oslib.o: ${BUILD_DIR}/config.h
rename.o: ${BUILD_DIR}/config.h
setlinebuf.o: ${BUILD_DIR}/config.h
strcasecmp.o: ${BUILD_DIR}/config.h
strerror.o: ${BUILD_DIR}/config.h
strtod.o: ${BUILD_DIR}/config.h
strtol.o: ${BUILD_DIR}/config.h
strtoul.o: ${BUILD_DIR}/config.h
vprint.o: ${BUILD_DIR}/config.h

clktck.o: ${topdir}/bashtypes.h

getcwd.o: ${topdir}/bashtypes.h ${topdir}/bashansi.h ${topdir}/maxpath.h
getcwd.o: ${POSIX_INC}/posixstat.h ${POSIX_INC}/posixdir.h
getcwd.o: ${POSIX_INC}/memalloc.h ${POSIX_INC}/ansi_stdlib.h

getenv.o: ${topdir}/bashansi.h ${topdir}/ansi_stdlib.h
getenv.o: ${topdir}/shell.h ${topdir}/bashjmp.h ${topdir}/posixjmp.h
getenv.o: ${topdir}/command.h ${topdir}/stdc.h ${topdir}/error.h
getenv.o: ${topdir}/general.h ${topdir}/bashtypes.h ${topdir}/variables.h
getenv.o: ${topdir}/array.h ${topdir}/hashlib.h ${topdir}/quit.h
getenv.o: ${topdir}/unwind_prot.h ${topdir}/dispose_cmd.h
getenv.o: ${topdir}/make_cmd.h ${topdir}/subst.h ${topdir}/sig.h
getenv.o: ${topdir}/pathnames.h ${topdir}/externs.h

itos.o: ${topdir}/bashansi.h ${topdir}/ansi_stdlib.h
itos.o: ${topdir}/shell.h ${topdir}/bashjmp.h ${topdir}/posixjmp.h
itos.o: ${topdir}/command.h ${topdir}/stdc.h ${topdir}/error.h
itos.o: ${topdir}/general.h ${topdir}/bashtypes.h ${topdir}/variables.h
itos.o: ${topdir}/array.h ${topdir}/hashlib.h ${topdir}/quit.h
itos.o: ${topdir}/unwind_prot.h ${topdir}/dispose_cmd.h
itos.o: ${topdir}/make_cmd.h ${topdir}/subst.h ${topdir}/sig.h
itos.o: ${topdir}/pathnames.h ${topdir}/externs.h

oslib.o: ${topdir}/bashtypes.h ${topdir}/bashansi.h ${topdir}/maxpath.h
oslib.o: ${topdir}/shell.h ${topdir}/bashjmp.h ${topdir}/posixjmp.h
oslib.o: ${topdir}/command.h ${topdir}/stdc.h ${topdir}/error.h
oslib.o: ${topdir}/general.h ${topdir}/bashtypes.h ${topdir}/variables.h
oslib.o: ${topdir}/array.h ${topdir}/hashlib.h ${topdir}/quit.h
oslib.o: ${topdir}/unwind_prot.h ${topdir}/dispose_cmd.h
oslib.o: ${topdir}/make_cmd.h ${topdir}/subst.h ${topdir}/sig.h
oslib.o: ${topdir}/pathnames.h ${topdir}/externs.h
oslib.o: ${POSIX_INC}/posixstat.h ${POSIX_INC}/filecntl.h
oslib.o: ${POSIX_INC}/ansi_stdlib.h

rename.o: ${topdir}/bashtypes.h ${topdir}/stdc.h

strcasecmp.o: ${topdir}/stdc.h ${topdir}/bashansi.h ${topdir}/ansi_stdlib.h

strerror.o: ${topdir}/bashtypes.h
strerror.o: ${topdir}/shell.h ${topdir}/bashjmp.h ${topdir}/posixjmp.h
strerror.o: ${topdir}/command.h ${topdir}/stdc.h ${topdir}/error.h
strerror.o: ${topdir}/general.h ${topdir}/bashtypes.h ${topdir}/variables.h
strerror.o: ${topdir}/array.h ${topdir}/hashlib.h ${topdir}/quit.h
strerror.o: ${topdir}/unwind_prot.h ${topdir}/dispose_cmd.h
strerror.o: ${topdir}/make_cmd.h ${topdir}/subst.h ${topdir}/sig.h
strerror.o: ${topdir}/pathnames.h ${topdir}/externs.h

strtod.o: ${topdir}/bashansi.h
strtod.o: ${POSIX_INC}/ansi_stdlib.h

strtol.o: ${topdir}/bashansi.h
strtol.o: ${POSIX_INC}/ansi_stdlib.h

strtoul.o: ${topdir}/bashansi.h
strtoul.o: ${POSIX_INC}/ansi_stdlib.h

include ../../../Makefile.targ
