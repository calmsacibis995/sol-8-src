/*
 *      Copyright (c) 1997 Sun Microsystems Inc
 *      All Rights Reserved.
 */

#ifndef	_MD5_CONSTS_H
#define	_MD5_CONSTS_H

#pragma ident	"@(#)md5_consts.h	1.1	97/11/19 SMI"

/*
 * as explained in md5c.c, loading 32-bit constants on a sparc is
 * expensive since it involves both a `sethi' and an `or'.  thus, we
 * instead use `ld' to load the constants from an array called
 * `md5_consts'.  however, on intel (and probably other processors),
 * it is cheaper to load the constant directly.  thus, the c code in
 * MD5Transform() uses the macro MD5_CONST() which either expands to a
 * constant or an array reference, depending on the architecture the
 * code is being compiled for.
 */

#include <sys/types.h>		/* uint32_t */

extern	const uint32_t		md5_consts[];

#if	defined(__sparc)
#define	MD5_CONST(x)		(md5_consts[x])
#else
#define	MD5_CONST(x)		(MD5_CONST_ ## x)
#endif

/* constants, as provided in RFC 1321 */

#define	MD5_CONST_0		(uint32_t)0xd76aa478
#define	MD5_CONST_1		(uint32_t)0xe8c7b756
#define	MD5_CONST_2		(uint32_t)0x242070db
#define	MD5_CONST_3		(uint32_t)0xc1bdceee
#define	MD5_CONST_4		(uint32_t)0xf57c0faf
#define	MD5_CONST_5		(uint32_t)0x4787c62a
#define	MD5_CONST_6		(uint32_t)0xa8304613
#define	MD5_CONST_7		(uint32_t)0xfd469501
#define	MD5_CONST_8		(uint32_t)0x698098d8
#define	MD5_CONST_9		(uint32_t)0x8b44f7af
#define	MD5_CONST_10		(uint32_t)0xffff5bb1
#define	MD5_CONST_11		(uint32_t)0x895cd7be
#define	MD5_CONST_12		(uint32_t)0x6b901122
#define	MD5_CONST_13		(uint32_t)0xfd987193
#define	MD5_CONST_14		(uint32_t)0xa679438e
#define	MD5_CONST_15		(uint32_t)0x49b40821
#define	MD5_CONST_16		(uint32_t)0xf61e2562
#define	MD5_CONST_17		(uint32_t)0xc040b340
#define	MD5_CONST_18		(uint32_t)0x265e5a51
#define	MD5_CONST_19		(uint32_t)0xe9b6c7aa
#define	MD5_CONST_20		(uint32_t)0xd62f105d
#define	MD5_CONST_21		(uint32_t)0x02441453
#define	MD5_CONST_22		(uint32_t)0xd8a1e681
#define	MD5_CONST_23		(uint32_t)0xe7d3fbc8
#define	MD5_CONST_24		(uint32_t)0x21e1cde6
#define	MD5_CONST_25		(uint32_t)0xc33707d6
#define	MD5_CONST_26		(uint32_t)0xf4d50d87
#define	MD5_CONST_27		(uint32_t)0x455a14ed
#define	MD5_CONST_28		(uint32_t)0xa9e3e905
#define	MD5_CONST_29		(uint32_t)0xfcefa3f8
#define	MD5_CONST_30		(uint32_t)0x676f02d9
#define	MD5_CONST_31		(uint32_t)0x8d2a4c8a
#define	MD5_CONST_32		(uint32_t)0xfffa3942
#define	MD5_CONST_33		(uint32_t)0x8771f681
#define	MD5_CONST_34		(uint32_t)0x6d9d6122
#define	MD5_CONST_35		(uint32_t)0xfde5380c
#define	MD5_CONST_36		(uint32_t)0xa4beea44
#define	MD5_CONST_37		(uint32_t)0x4bdecfa9
#define	MD5_CONST_38		(uint32_t)0xf6bb4b60
#define	MD5_CONST_39		(uint32_t)0xbebfbc70
#define	MD5_CONST_40		(uint32_t)0x289b7ec6
#define	MD5_CONST_41		(uint32_t)0xeaa127fa
#define	MD5_CONST_42		(uint32_t)0xd4ef3085
#define	MD5_CONST_43		(uint32_t)0x4881d05
#define	MD5_CONST_44		(uint32_t)0xd9d4d039
#define	MD5_CONST_45		(uint32_t)0xe6db99e5
#define	MD5_CONST_46		(uint32_t)0x1fa27cf8
#define	MD5_CONST_47		(uint32_t)0xc4ac5665
#define	MD5_CONST_48		(uint32_t)0xf4292244
#define	MD5_CONST_49		(uint32_t)0x432aff97
#define	MD5_CONST_50		(uint32_t)0xab9423a7
#define	MD5_CONST_51		(uint32_t)0xfc93a039
#define	MD5_CONST_52		(uint32_t)0x655b59c3
#define	MD5_CONST_53		(uint32_t)0x8f0ccc92
#define	MD5_CONST_54		(uint32_t)0xffeff47d
#define	MD5_CONST_55		(uint32_t)0x85845dd1
#define	MD5_CONST_56		(uint32_t)0x6fa87e4f
#define	MD5_CONST_57		(uint32_t)0xfe2ce6e0
#define	MD5_CONST_58		(uint32_t)0xa3014314
#define	MD5_CONST_59		(uint32_t)0x4e0811a1
#define	MD5_CONST_60		(uint32_t)0xf7537e82
#define	MD5_CONST_61		(uint32_t)0xbd3af235
#define	MD5_CONST_62		(uint32_t)0x2ad7d2bb
#define	MD5_CONST_63		(uint32_t)0xeb86d391

/* initialization constants, as given in RFC 1321. used in MD5Init */

#define	MD5_INIT_CONST_1	0x67452301
#define	MD5_INIT_CONST_2	0xefcdab89
#define	MD5_INIT_CONST_3	0x98badcfe
#define	MD5_INIT_CONST_4	0x10325476

/* shift constants, as given in RFC 1321.  used in MD5Transform */

#define	MD5_SHIFT_11		7
#define	MD5_SHIFT_12		12
#define	MD5_SHIFT_13		17
#define	MD5_SHIFT_14		22
#define	MD5_SHIFT_21		5
#define	MD5_SHIFT_22		9
#define	MD5_SHIFT_23		14
#define	MD5_SHIFT_24		20
#define	MD5_SHIFT_31		4
#define	MD5_SHIFT_32		11
#define	MD5_SHIFT_33		16
#define	MD5_SHIFT_34		23
#define	MD5_SHIFT_41		6
#define	MD5_SHIFT_42		10
#define	MD5_SHIFT_43		15
#define	MD5_SHIFT_44		21

#endif /* _MD5_CONSTS_H */
