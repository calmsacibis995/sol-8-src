#ident	"@(#)Makefile	1.1	99/10/08 SMI"
#
# This is the Makefile for the examples subdirectory of readline. -*- text -*-
#
EXECUTABLES = fileman rltest rl
CFLAGS  = -g -I../.. -I..
LDFLAGS = -g -L..

.c.o:
	$(CC) $(CFLAGS) -c $<

all: $(EXECUTABLES)


rl: rl.o
	$(CC) $(LDFLAGS) -o $@ rl.o -lreadline -ltermcap

fileman: fileman.o
	$(CC) $(LDFLAGS) -o $@ fileman.o -lreadline -ltermcap

rltest: rltest.o
	$(CC) $(LDFLAGS) -o $@ rltest.o -lreadline -ltermcap

fileman.o: fileman.c
rltest.o: rltest.c
rl.o: rl.c
