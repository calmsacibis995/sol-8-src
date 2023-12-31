#
# uts/sun4u/unix/Makefile
# Copyright (c) 1991 by Sun Microsystems, Inc.
#
#ident	"@(#)Makefile	1.23	98/06/26 SMI"
#
#	This makefile drives the production of /unix (and unix.o).
#
#	sun4u implementation architecture dependent
#

#
#	Path to the base of the uts directory tree (usually /usr/src/uts).
#
UTSBASE	= ../..

#
#	Define the module and object file sets.
#
UNIX		= unix
OBJECTS		= $(SPECIAL_OBJS:%=$(OBJS_DIR)/%) \
		  $(CORE_OBJS:%=$(OBJS_DIR)/%) \
		  $(MACH_NOT_YET_KMODS:%=$(OBJS_DIR)/%)
LINTS		= $(SPECIAL_OBJS:%.o=$(LINTS_DIR)/%.ln) \
		  $(CORE_OBJS:%.o=$(LINTS_DIR)/%.ln) \
		  $(MACH_NOT_YET_KMODS:%.o=$(LINTS_DIR)/%.ln) \
		  $(LINTS_DIR)/vers.ln \
		  $(LINTS_DIR)/modstubs.ln
ROOTMODULE	= $(ROOT_PSM_KERN_DIR)/$(UNIX)
UNIX_BIN	= $(OBJS_DIR)/$(UNIX)

KRTLD_32	= misc/krtld
KRTLD_64	= misc/$(SUBDIR64)/krtld
KRTLD		= $(KRTLD_$(CLASS))

LIBS		= $(GENLIB) $(PLATLIB) $(CPULIB)

GENUNIX		= genunix
GENUNIX_DIR	= ../$(GENUNIX)
GENOPTS		= -L $(GENUNIX_DIR)/$(OBJS_DIR) -l $(GENUNIX)

CPU_DIR		= .
CPUOPTS		= -L $(CPU_DIR)/$(OBJS_DIR) -l $(CPUNAME)

PLAT_DIR	= ../platmod
PLATOPTS	= -L $(PLAT_DIR)/$(OBJS_DIR) -l $(PLATMOD)

LIBOPTS		= $(GENOPTS) $(PLATOPTS) $(CPUOPTS)

#
#	Include common rules.
#
include $(UTSBASE)/sun4u/Makefile.sun4u

#
#	Define targets
#
ALL_TARGET	= $(UNIX_BIN)
LINT_TARGET	= $(LINT_LIB)
INSTALL_TARGET	= $(UNIX_BIN) $(ROOTMODULE)

#
#	This is UNIX_DIR. Use a short path.
#
UNIX_DIR	= .

#
#	Overrides
#
CLEANFILES	+= $(UNIX_O) $(MODSTUBS_O) $(OBJS_DIR)/vers.c \
		   $(OBJS_DIR)/vers.o $(OBJS_DIR)/$(CPU_OBJ) $(CPULIB)
CLOBBERFILES	= $(CLEANFILES) $(UNIX_BIN)
CLEANLINTFILES	+= $(LINT_LIB)

#
# lint pass one enforcement 
# Turn on doubleword alignment for 64 bit counter timer registers
#
CFLAGS += -v -dalign

#
#	Default build targets.
#
.KEEP_STATE:

def:		$(DEF_DEPS)

all:		$(ALL_DEPS)

clean:		$(CLEAN_DEPS)

clobber:	$(CLOBBER_DEPS)

lint:		$(LINT_DEPS)

clean.lint:	$(CLEAN_LINT_DEPS)

install:  	$(INSTALL_DEPS)

$(UNIX_BIN):	$(UNIX_O) $(MODSTUBS_O) $(MAPFILE) $(LIBS)
	$(LD) -dy -b -o $@ -e _start -I $(KRTLD) -M $(MAPFILE) \
	$(UNIX_O) $(MODSTUBS_O) $(LIBOPTS)
	$(POST_PROCESS)

symcheck:	$(UNIX_O) $(MODSTUBS_O) $(LIBS)
	$(LD) -dy -b -o $(SYM_MOD) -M $(MAPFILE) \
	$(UNIX_O) $(MODSTUBS_O) $(LIBOPTS)

#$(UNIX).glom:	$(UNIX)
#	${LD} -dy -b -z nodefs -o $@ -e _start -I $(KRTLD) -M $(MAPFILE) \
#	$(UNIX_O) $(MODSTUBS_O) $(GENUNIX_DIR)/$(GENUNIX)
#	$(POST_PROCESS)

$(UNIX_O):	$(OBJECTS) $(OBJS_DIR)/vers.o
	$(LD) -r -o $@ $(OBJECTS) $(OBJS_DIR)/vers.o

#
#	Special rules for generating assym.h for inclusion in assembly files.
#
$(DSF_DIR)/$(OBJS_DIR)/assym.h:	FRC
	@cd $(DSF_DIR); $(MAKE) all.targ

$(GENLIB):	FRC
	@(cd $(GENLIB_DIR); pwd; $(MAKE) all.targ)
	@pwd

$(PLATLIB):
	?@(cd $(PLAT_DIR); pwd; $(MAKE) all.targ)
	?@pwd

$(CPULIB):	$(OBJS_DIR)/$(CPU_OBJ)
	$(LD) -o $@ -G -h 'cpu/$$CPU' $(OBJS_DIR)/$(CPU_OBJ)

#
#	The global lint target builds the kernel lint library (llib-lunix.ln)
#	which is equivalent to a lint of /unix.o. Then all kernel modules for
#	this architecture are linted against the kernel lint library.
#
#	Note:	lint errors in the kernel lint library will be repeated for
#		each module. It is important that the kernel lint library
#		be clean to keep the textual output to a reasonable level.
#

$(LINT_LIB):	$(LINT_LIB_DIR) $(LINTS)
	@-$(ECHO) "\n$(UNIX): (library construction):"
	@$(LINT) -o $(UNIX) $(LINTFLAGS) $(LINTS)
	@$(MV) $(@F) $@

lintlib:	$(LINT_DEPS)

#
#	Include common targets.
#
include $(UTSBASE)/sun4u/Makefile.targ

