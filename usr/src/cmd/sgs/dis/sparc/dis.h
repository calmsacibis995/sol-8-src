/*
 * Copyright (c) 1997, by Sun Microsystems, Inc.
 * All rights reserved.
 */
#pragma ident	"@(#)dis.h	1.5	97/09/07 SMI"

/*
 *	This is the header file for the sparc disassembler.
 *	The information contained in the first part of the file
 *	is common to each version, while the last part is dependent
 *	on the particular machine architecture being used.
 */

#ifndef	_DIS_H
#define	_DIS_H 1

#include	<unistd.h>
#include	<sys/types.h>
#include	<stdio.h>
#include	<libelf.h>
#include	<stdlib.h>
#include	<malloc.h>
#include	<fcntl.h>
#include	<strings.h>
#include	<sys/isa_defs.h>

#include	 <libelf.h>
#include	 <gelf.h>
#include	"structs.h"
#include	"sgs.h"

#define	APNO	10
#define	FPNO	9
#define	PCNO	15
#define	NCPS	10	/* number of chars per symbol	*/
#define	NHEX	80	/* max # chars in object per line */
#define	NLINE	1024	/* max # chars in mnemonic per line */
#define	FAIL	0
#define	LEAD	1
#define	NOLEAD	0

/*
 * used in tables.c to indicate  that the 'indirect' field of term
 * 'instable' terminates - no pointer. Is also checked in 'dis_text()' in
 * bits.c.
 */
#define	TERM	0

/*
 * used in string compare operation in 'disassembly' routine
 */
#define	STRNGEQ 0

/*
 *	The following are constants that are used in the disassembly
 *	of floating point immediate operands.
 */
#define	NOTFLOAT	0
#define	FPSINGLE	1
#define	FPDOUBLE	2

/*
 * 2 ** 23 used in conversion of floating point object to
 * a decimal number in _utls.c
 */
#define	TWO_23	8388608

/*
 * 2 ** 32 also used in floating point number conversion in _utls.c
 */
#define	TWO_32	4294967296.

/*
 * 2 ** 52 also used in floating point conversion routines.
 */
#define	TWO_52	4503599627370496.

/*
 * bias on 8 bit exponent of floating  point number in _utls.c
 */
#define	BIAS	127

/*
 * bias on 11 bit exponent of double precision floating point number
 * in _utls.c
 */
#define	DBIAS	1023
/*
 * The following are the 7 posssible types of floating point immediate
 * operands. These are the possible values of [s|d]fpconv() which
 * are in _utls.c.
 */
#define	NOTANUM		0
#define	NEGINF		1
#define	INFINITY	2
#define	ZERO		3
#define	NEGZERO		4
#define	FPNUM		5
#define	FPBIGNUM	6

/*
 *	This is a symbolic representation of all support processor
 *	identifiers.
 */

#define	MAU_ID		0

/*
 *	This is the structure that will be used for storing all the
 *	op code information.  The structure values themselves are
 *	in 'tables.c'.
 */

struct	instable {
	char		name[NCPS];
	unsigned	class;
};
/*
 *	This is the structure that will be used for storing all the
 *	address modification information.  The structure values
 *	themselves are in 'tables.c'.
 */
struct	formtable {
	char		name[NCPS];
	unsigned	typ;
};

typedef struct {
	char *name;
	unsigned long op;
	unsigned long mask;
	int type;
	unsigned long vermask;
} INST;

typedef unsigned long Instruction;
typedef char *(*FUNCPTR)(GElf_Addr);
typedef unsigned char FORMAT;

/*
 *	NOTE:	the following information in this file must be changed
 *		between the different versions of the disassembler.
 *
 *	These are the instruction classes as they appear in
 *	'tables.c'.  Here they are given numerical values
 *	for use in the actual disassembly of an object file.
 */

#define	UNKNOWN		0
#define	OPRNDS0		1	/* instructions with no operands */
#define	OPRNDS1		2	/* instructions with 1 operand */
#define	OPRNDS2		3	/* instructions with 2 operands	*/
#define	OPRNDS3		4	/* instructions with 3 operands	*/
#define	OPRNDS4		5	/* instructions with 4 operands	*/
#define	JUMP		6	/* "jump" instruction */
#define	JUMPSB		7	/* "jumpsb" instruction */
#define	JUMP1		8	/* 1 byte offset jump instruction */
#define	JUMP2		9	/* 2 byte offset jump instruction */
#define	BSBB		10	/* "BSBB" instruction */
#define	BSBH		11	/* "BSBB" instruction */
#define	NOOP8		12	/* 8 bit offset (no operands) */
#define	NOOP16		13	/* 16 bit offset (no operands) */
#define	CALL		14	/* "call" instruction */
#define	EXT		15	/* "extendop" instruction */
#define	AD1OP3		17	/* 4 operands but add 1 to 3nd op */
#define	SFPOPS2		18	/* single floating point, 2 operands */
#define	SFPOPS3		19	/* single floating point, 3 operands */
#define	MACRO		20	/* macro-rom instruction */
#define	DFPOPS2		21	/* double floating point, 2 operands */
#define	DFPOPS3		22	/* double floating point, 3 operands */
#define	SPRTOP0		23	/* support processor inst.0 operands */
#define	SPRTOP1		24	/* support processor inst. 1 operand */
#define	SPRTOP2		25	/* support processor inst. 2 operand */

/*
 *	These are the address modification types as they appear in
 *	'tables.c'.  Here they are given numerical values for
 *	use in the actual disassembly of an object file.
 */

#define	IM6	1	/* immediate 6 bit field */
#define	R	2	/* register */
#define	RDF	3	/* register deferred (indirect) */
#define	R4	4	/* register and 4 bit displacement */
#define	IMB	5	/* immediate byte */
#define	IMH	6	/* immediate half word */
#define	IMW	7	/* immediate word */
#define	BDB	8	/* base (register) and byte displacement */
#define	BDH	9	/* base (register) and half word disp */
#define	BDW	10	/* base (register) and word displacement */
#define	BDBDF	11	/* base and byte displacement deferred */
#define	BDHDF	12	/* base and half word displacement deferred */
#define	BDWDF	13	/* base and word displacement deferred */
#define	EXPSB	14	/* expanded signed byte	*/
#define	EXPUB	15	/* expanded unsigned byte */
#define	EXPSH	16	/* expanded signed half word */
#define	EXPUH	17	/* expanded unsigned half word */
#define	EXPSW	18	/* expanded signed word	*/
#define	EXPUW	19	/* expanded unsigned word */
#define	IM4	20	/* immediate 4 bit field */
#define	ABAD	21	/* absolute address */
#define	ABADDF	22	/* absolute address deferred */

/*
 * from bits.c
 */
void dis_text(GElf_Shdr * shdr);
void get1byte(void);
long getnextword(void);

/*
 * from disasm.c
 */
char *disassemble(Instruction, GElf_Addr);
char  *toSymbolic(GElf_Addr);
INST * dsmFind(Instruction);
void dsmError(void);
FORMAT *findFmt(int);

#endif /* _DIS_H */
