/*	Copyright (c) 1984, 1986, 1987, 1988, 1989 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ifndef	_SYS_TTCOMPAT_H
#define	_SYS_TTCOMPAT_H

#pragma ident	"@(#)ttcompat.h	1.15	97/10/22 SMI"	/* SVr4.0 1.1 */

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * BSD/XENIX/V7 ttcompat module header file
 */

/*
 * Old-style terminal state.
 */
typedef struct {
	int	t_flags;		/* flags */
	char	t_ispeed, t_ospeed;	/* speeds */
	char	t_erase;		/* erase last character */
	char	t_kill;			/* erase entire line */
	char	t_intrc;		/* interrupt */
	char	t_quitc;		/* quit */
	char	t_startc;		/* start output */
	char	t_stopc;		/* stop output */
	char	t_eofc;			/* end-of-file */
	char	t_brkc;			/* input delimiter (like nl) */
	char	t_suspc;		/* stop process signal */
	char	t_dsuspc;		/* delayed stop process signal */
	char	t_rprntc;		/* reprint line */
	char	t_flushc;		/* flush output (toggles) */
	char	t_werasc;		/* word erase */
	char	t_lnextc;		/* literal next character */
	int	t_xflags;		/* XXX extended flags */
} compat_state_t;

/*
 * Per-tty structure.
 */
typedef struct {
	mblk_t	*t_iocpending;		/* ioctl pending successful */
					/* allocation */
	compat_state_t t_curstate;	/* current emulated state */
	struct sgttyb t_new_sgttyb;	/* new sgttyb from TIOCSET[PN] */
	struct tchars t_new_tchars;	/* new tchars from TIOCSETC */
	struct ltchars t_new_ltchars;	/* new ltchars from TIOCSLTC */
	int	t_new_lflags;		/* new lflags from TIOCLSET/LBIS/LBIC */
	int	t_state;		/* state bits */
	int	t_iocid;		/* ID of "ioctl" we handle specially */
	int	t_ioccmd;		/* ioctl code for that "ioctl" */
	bufcall_id_t t_bufcallid;	/* ID from qbufcall */
	intptr_t t_arg;			/* third argument to ioctl */
} ttcompat_state_t;


#define	TS_FREE	 0x00	/* not in use */
#define	TS_INUSE 0x01	/* allocated */
#define	TS_W_IN	 0x02	/* waiting for an M_IOCDATA response to an */
			/* M_COPYIN request */
#define	TS_W_OUT 0x04	/* waiting for an M_IOCDATA response to an */
			/* M_COPYOUT request */
#define	TS_IOCWAIT 0x08	/* waiting for an M_IOCACK/M_IOCNAK from downstream */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_TTCOMPAT_H */