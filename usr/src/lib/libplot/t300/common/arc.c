/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

/*
 * Copyright (c) 1997 by Sun Microsystems, Inc.
 * All rights reserved
 */

#pragma ident	"@(#)arc.c	1.8	97/10/29 SMI"	/* SVr4.0 1.4	*/
/*LINTLIBRARY*/

#include <stdlib.h>
#include <plot.h>
#include <math.h>
#include "con.h"

static int quad_l(short, short, short, short);
static int del = 20;

static void
step(int d)
{
	del = d;
}

void
arc(short x, short y, short x0, short y0, short x1, short y1)
{
	double pc;
	short flg, m, xc, yc, xs, ys, qs, qf, qt, qtctr = 0;
	short m0, m1;
	float dx, dy, r;
	char use;
	dx = x - x0;
	dy = y - y0;
	r = (dx * dx) + (dy * dy);
	pc = r;
	pc = sqrt(pc);
	flg = pc / 4;
	if (flg == 0)
		step(1);
	else if (flg < del)
		step(flg);
	xc = xs = x0;
	yc = ys = y0;
	move(xs, ys);
	if ((x0 == x1) && (y0 == y1))
		flg = 0;
	else flg = 1;
	qs = quad_l(x, y, x0, y0);
	qf = quad_l(x, y, x1, y1);
	if ((abs(x - x1)) < (abs(y - y1))) {
		use = 'x';
		if ((qs == 2) || (qs == 3))
			m = -1;
		else m = 1;
	} else {
		use = 'y';
		if (qs > 2)
			m = -1;
		else m = 1;
	}
	if (qs == qf) {
		m0 = (y0 - y) / (x0 - x);
		m1 = (y1 - y) / (x1 - x);
		if (m0 >= m1)
			qt = 4;
		else qt = 0;
	} else if ((qt = qf - qs) < 0)
		qt += 4;
	/* LINTED */
	while (1) {
		switch (use) {
		case 'x':
			if ((qs == 2) || (qs == 3))
				yc -= del;
			else yc += del;
			dy = yc - y;
			pc = r - dy * dy;
			xc = m * sqrt(pc) + x;
			if (((x < xs) && (x >= xc)) ||
			    ((x > xs) && (x <= xc)) ||
			    ((y < ys) && (y >= yc)) ||
			    ((y > ys) && (y <=  yc))) {
				if (++qtctr > qt)
					return;
				if (++qs > 4)
					qs = 1;
				if ((qs == 2) || (qs == 3))
					m = -1;
				else m = 1;
				flg = 1;
			}
			cont(xc, yc);
			xs = xc;
			ys = yc;
			if ((qs == qf) && (flg == 1))
				switch (qf) {
				case 3:
				case 4:
					if (xs >= x1)
						return;
					continue;
				case 1:
				case 2:
					if (xs <= x1)
						return;
				}
			continue;
		case 'y':
			if (qs > 2)
				xc += del;
			else xc -= del;
			dx = xc - x;
			pc = r - dx * dx;
			yc = m * sqrt(pc) + y;
			if (((x < xs) && (x >= xc)) ||
			    ((x > xs) && (x <= xc)) ||
			    ((y < ys) && (y >= yc)) ||
			    ((y > ys) && (y <= yc))) {
				if (++qtctr > qt)
					return;
				if (++qs > 4)
					qs = 1;
				if (qs > 2)
					m = -1;
				else m = 1;
				flg = 1;
			}
			cont(xc, yc);
			xs = xc;
			ys = yc;
			if ((qs == qf) && (flg == 1))
				switch (qs) {
				case 1:
				case 4:
					if (ys >= y1)
						return;
					continue;
				case 2:
				case 3:
					if (ys <= y1)
						return;
				}
		}
	}
}

/*
 * these is a local function not to be confused with the
 * system wide quad
 */
static int
quad_l(short x, short y, short xp, short yp)
{
	if (x < xp)
		if (y <= yp)
			return (1);
		else return (4);
	else if (x > xp)
		if (y < yp)
			return (2);
		else return (3);
	else if (y < yp)
		return (2);
	else return (4);
}