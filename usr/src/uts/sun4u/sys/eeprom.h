/*
 * Copyright (c) 1991-1998 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ifndef _SYS_EEPROM_H
#define	_SYS_EEPROM_H

#pragma ident	"@(#)eeprom.h	1.7	98/01/06 SMI"

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * The EEPROM is part of the Mostek MK48T02 clock chip. The EEPROM
 * is 2K, but the last 8 bytes are used as the clock, and the 32 bytes
 * before that emulate the ID prom. There is no
 * recovery time necessary after writes to the chip.
 */
#ifndef _ASM
#include <sys/types.h>
struct ee_soft {
	ushort_t ees_wrcnt[3];		/* write count (3 copies) */
	ushort_t ees_nu1;		/* not used */
	uchar_t	ees_chksum[3];		/* software area checksum (3 copies) */
	uchar_t	ees_nu2;		/* not used */
	uchar_t	ees_resv[0xd8-0xc];	/* XXX - figure this out sometime */
};

extern caddr_t v_eeprom_addr;
#define	EEPROM		((struct eeprom *)v_eeprom_addr)
#define	IDPROM_ADDR	(v_eeprom_addr+EEPROM_SIZE) /* virt addr of idprom */
#endif /* !_ASM */

#define	EEPROM_SIZE	0x1fd8		/* size of eeprom in bytes */

/*
 * ID prom constants. They are included here because the ID prom is
 * emulated by stealing 20 bytes of the eeprom.
 */
#define	IDPROMSIZE	0x20			/* size of ID prom, in bytes */

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_EEPROM_H */
