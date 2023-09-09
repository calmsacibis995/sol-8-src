/*
 * Copyright (c) 1993, 1994 Sun Microsystems, Inc. All rights reserved.
 */

#ident	"@(#)_ctype.c	1.5	94/05/23 SMI\n"

/*
 * Solaris Primary Boot Subsystem - Support Library Routine
 *===========================================================================
 * Provides minimal services for the real-mode environment that the operating 
 * system would normally supply.
 *
 *   Function name:	_ctype  (_ctype.c)
 *
 *   Calling Syntax:	typically, a single input argument as in
 *			"isdigit ( testchar )"; see ctype.h.
 *
 *   Description:	Character classification tables.
 *        		isalpha, isupper, islower, isdigit, isxdigit, isalnum,
 *			isspace, ispunct, isprint, isgraph, iscntrl, isascii,
 *			_toupper, _tolower, _toascii; this family of
 *			character-type functions interrogates for a specific
 *			character type, and returns 1 if true, 0 otherwise.
 *
 */

#include <ctype.h>
#include <_locale.h>

unsigned char _ctype[SZ_TOTAL] =
{
        0, /*EOF*/
        _C,     _C,     _C,     _C,     _C,     _C,     _C,     _C,
        _C,     _S|_C,  _S|_C,  _S|_C,  _S|_C,  _S|_C,  _C,     _C,
        _C,     _C,     _C,     _C,     _C,     _C,     _C,     _C,
        _C,     _C,     _C,     _C,     _C,     _C,     _C,     _C,
        _S|_B,  _P,     _P,     _P,     _P,     _P,     _P,     _P,
        _P,     _P,     _P,     _P,     _P,     _P,     _P,     _P,
        _N|_X,  _N|_X,  _N|_X,  _N|_X,  _N|_X,  _N|_X,  _N|_X,  _N|_X,
        _N|_X,  _N|_X,  _P,     _P,     _P,     _P,     _P,     _P,
        _P,     _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U|_X,  _U,
        _U,     _U,     _U,     _U,     _U,     _U,     _U,     _U,
        _U,     _U,     _U,     _U,     _U,     _U,     _U,     _U,
        _U,     _U,     _U,     _P,     _P,     _P,     _P,     _P,
        _P,     _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L|_X,  _L,
        _L,     _L,     _L,     _L,     _L,     _L,     _L,     _L,
        _L,     _L,     _L,     _L,     _L,     _L,     _L,     _L,
        _L,     _L,     _L,     _P,     _P,     _P,     _P,     _C,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,

/* tolower()  and toupper() conversion table */ 0,
        0,      1,      2,      3,      4,      5,      6,      7,
        8,      9,      10,     11,     12,     13,     14,     15,
        16,     17,     18,     19,     20,     21,     22,     23,
        24,     25,     26,     27,     28,     29,     30,     31,
        32,     33,     34,     35,     36,     37,     38,     39,
        40,     41,     42,     43,     44,     45,     46,     47,
        48,     49,     50,     51,     52,     53,     54,     55,
        56,     57,     58,     59,     60,     61,     62,     63,
        64,     97,     98,     99,     100,    101,    102,    103,
        104,    105,    106,    107,    108,    109,    110,    111,
        112,    113,    114,    115,    116,    117,    118,    119,
        120,    121,    122,    91,     92,     93,     94,     95,
        96,     65,     66,     67,     68,     69,     70,     71,
        72,     73,     74,     75,     76,     77,     78,     79,
        80,     81,     82,     83,     84,     85,     86,     87,
        88,     89,     90,     123,    124,    125,    126,    127,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
        0,      0,      0,      0,      0,      0,      0,      0,
/* CSWIDTH information */
        1,      0,      0,      1,      0,      0,      1,
};
