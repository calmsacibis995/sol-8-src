#ident	"@(#)Makefile	1.1	99/10/08 SMI"
#
# Generated automatically from Makefile.in by configure.
#
# Simple makefile for the sample loadable builtins
#
#
# Include some boilerplate Gnu makefile definitions.

topdir = ../..
BUILD_DIR = ../..
srcdir = .
VPATH = .:.

include ../../../Makefile.cmd

CC = gcc
RM = rm -f

SHELL = /bin/sh

host_os = solaris2.8
host_cpu = $(MACH)
host_vendor = sun

#
# These values are generated for configure by ${topdir}/support/shobj-conf.
# If your system is not supported by that script, but includes facilities for
# dynamic loading of shared objects, please update the script and send the
# changes to bash-maintainers@gnu.org.
#
SHOBJ_CC = $(CC)
SHOBJ_CFLAGS = -K pic
SHOBJ_LD = ${CC}
SHOBJ_LDFLAGS = -G
SHOBJ_XLDFLAGS = 
SHOBJ_LIBS = 
SHOBJ_STATUS = supported

INC = -I. -I.. -I$(topdir) -I$(topdir)/lib -I$(topdir)/builtins \
      -I$(BUILD_DIR) -I$(BUILD_DIR)/lib -I$(BUILD_DIR)/builtins

.c.o:
	$(SHOBJ_CC) $(SHOBJ_CFLAGS) $(CFLAGS) $(INC) -c -o $@ $<


ALLPROG = print truefalse sleep pushd finfo logname basename dirname \
	  tty pathchk tee head mkdir rmdir sprintf printenv id whoami \
	  uname sync push ln unlink
OTHERPROG = necho getconf hello cat

all:	$(SHOBJ_STATUS)

supported:	$(ALLPROG)
others:		$(OTHERPROG)

unsupported:
	@echo "Your system (${host_os}) is not supported by the"
	@echo "${topdir}/support/shobj-conf script."
	@echo "If your operating system provides facilities for dynamic"
	@echo "loading of shared objects using the dlopen(3) interface,"
	@echo "please update the script and re-run configure.
	@echo "Please send the changes you made to bash-maintainers@gnu.org"
	@echo "for inclusion in future bash releases."

everything: supported others

sprintf: sprintf.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ sprintf.o $(SHOBJ_LIBS)

print:	print.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ print.o $(SHOBJ_LIBS)

necho:	necho.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ necho.o $(SHOBJ_LIBS)

getconf: getconf.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ getconf.o $(SHOBJ_LIBS)

hello:	hello.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ hello.o $(SHOBJ_LIBS)

truefalse: truefalse.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ truefalse.o $(SHOBJ_LIBS)

sleep:	sleep.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ sleep.o $(SHOBJ_LIBS)

finfo:	finfo.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ finfo.o $(SHOBJ_LIBS)

cat:	cat.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ cat.o $(SHOBJ_LIBS)

logname:	logname.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ logname.o $(SHOBJ_LIBS)

basename:	basename.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ basename.o $(SHOBJ_LIBS)

dirname:	dirname.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ dirname.o $(SHOBJ_LIBS)

tty:	tty.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ tty.o $(SHOBJ_LIBS)

pathchk:	pathchk.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ pathchk.o $(SHOBJ_LIBS)

tee:	tee.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ tee.o $(SHOBJ_LIBS)

mkdir:	mkdir.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ mkdir.o $(SHOBJ_LIBS)

rmdir:	rmdir.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ rmdir.o $(SHOBJ_LIBS)

head:	head.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ head.o $(SHOBJ_LIBS)

printenv:	printenv.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ printenv.o $(SHOBJ_LIBS)

id:	id.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ id.o $(SHOBJ_LIBS)

whoami:	whoami.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ whoami.o $(SHOBJ_LIBS)

uname:	uname.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ uname.o $(SHOBJ_LIBS)

sync:	sync.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ sync.o $(SHOBJ_LIBS)

push:	push.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ push.o $(SHOBJ_LIBS)

ln:	ln.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ ln.o $(SHOBJ_LIBS)

unlink:	unlink.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ unlink.o $(SHOBJ_LIBS)


# pushd is a special case.  We use the same source that the builtin version
# uses, with special compilation options.
#
pushd.c:	${topdir}/builtins/pushd.def
	$(RM) $@
	${BUILD_DIR}/builtins/mkbuiltins -D ${topdir}/builtins ${topdir}/builtins/pushd.def

pushd.o:	pushd.c
	$(RM) $@
	$(SHOBJ_CC) -DPUSHD_AND_POPD -DLOADABLE_BUILTIN $(SHOBJ_CFLAGS) $(CFLAGS) $(INC) -c -o $@ $<

pushd:	pushd.o
	$(SHOBJ_LD) $(SHOBJ_LDFLAGS) $(SHOBJ_XLDFLAGS) -o $@ pushd.o $(SHOBJ_LIBS)

clean:
	$(RM) $(ALLPROG) $(OTHERPROG) *.o

mostlyclean:	clean

distclean maintainer-clean: clean
	$(RM) Makefile pushd.c

print.o: print.c
truefalse.o: truefalse.c
sleep.o: sleep.c
finfo.o: finfo.c
logname.o: logname.c
basename.o: basename.c
dirname.o: dirname.c
tty.o: tty.c
pathchk.o: pathchk.c
tee.o: tee.c
head.o: head.c
rmdir.o: rmdir.c
sprintf.o: sprintf.c
necho.o: necho.c
getconf.o: getconf.c
hello.o: hello.c
cat.o: cat.c
printenv.o: printenv.c
id.o: id.c
whoami.o: whoami.c
uname.o: uname.c
sync.o: sync.c
push.o: push.c
mkdir.o: mkdir.c

include ../../../Makefile.targ
