/*
 * Copyright (c) 1991-1994, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma ident	"@(#)prom_mem.c	1.5	97/06/30 SMI"

/*
 * This file contains platform-dependent memory support routines,
 * suitable for memory methods with 2-cell physical addresses.
 * Use of these routines makes the caller platform-dependent,
 * since the caller assumes knowledge of the physical layout of
 * the machines address space.  Generic programs should use the
 * standard client interface memory allocators.
 */

#include <sys/promif.h>
#include <sys/promimpl.h>

ihandle_t
prom_memory_ihandle(void)
{
	static ihandle_t imemory;

	if (imemory != (ihandle_t)0)
		return (imemory);

	if (prom_getproplen(prom_chosennode(), "memory") != sizeof (ihandle_t))
		return (imemory = (ihandle_t)-1);

	(void) prom_getprop(prom_chosennode(), "memory", (caddr_t)(&imemory));
	return (imemory);
}

/*
 * Allocate physical memory, unmapped and possibly aligned.
 * Returns 0: Success; Non-zero: failure.
 * Returns *physaddr only if successful.
 *
 * This routine is suitable for platforms with 2-cell physical addresses
 * and a single size cell in the "memory" node.
 */
int
prom_allocate_phys(size_t size, u_int align, unsigned long long *physaddr)
{
	cell_t ci[10];
	int rv;
	ihandle_t imemory = prom_memory_ihandle();

	if ((imemory == (ihandle_t)-1))
		return (-1);

	if (align == 0)
		align = (u_int)1;

	ci[0] = p1275_ptr2cell("call-method");	/* Service name */
	ci[1] = (cell_t)4;			/* #argument cells */
	ci[2] = (cell_t)3;			/* #result cells */
	ci[3] = p1275_ptr2cell("claim");	/* Arg1: Method name */
	ci[4] = p1275_ihandle2cell(imemory);	/* Arg2: memory ihandle */
	ci[5] = p1275_uint2cell(align);		/* Arg3: SA1: align */
	ci[6] = p1275_size2cell(size);		/* Arg4: SA2: size */

	promif_preprom();
	rv = p1275_cif_handler(&ci);
	promif_postprom();

	if (rv != 0)
		return (rv);
	if (p1275_cell2int(ci[7]) != 0)		/* Res1: Catch result */
		return (-1);

	*physaddr = p1275_cells2ull(ci[8], ci[9]);
				/* Res2: SR1: phys.hi ... Res3: SR2: phys.lo */
	return (0);
}

/*
 * Claim a region of physical memory, unmapped.
 * Returns 0: Success; Non-zero: failure.
 *
 * This routine is suitable for platforms with 2-cell physical addresses
 * and a single size cell in the "memory" node.
 */
int
prom_claim_phys(size_t size, unsigned long long physaddr)
{
	cell_t ci[10];
	int rv;
	ihandle_t imemory = prom_memory_ihandle();

	if ((imemory == (ihandle_t)-1))
		return (-1);

	ci[0] = p1275_ptr2cell("call-method");	/* Service name */
	ci[1] = (cell_t)6;			/* #argument cells */
	ci[2] = (cell_t)1;			/* #result cells */
	ci[3] = p1275_ptr2cell("claim");	/* Arg1: Method name */
	ci[4] = p1275_ihandle2cell(imemory);	/* Arg2: mmu ihandle */
	ci[5] = 0;				/* Arg3: SA1: align */
	ci[6] = p1275_size2cell(size);		/* Arg4: SA2: len */
	ci[7] = p1275_ull2cell_high(physaddr);	/* Arg5: SA3: phys.hi */
	ci[8] = p1275_ull2cell_low(physaddr);	/* Arg6: SA4: phys.lo */

	promif_preprom();
	rv = p1275_cif_handler(&ci);
	promif_postprom();

	if (rv != 0)
		return (rv);
	if (p1275_cell2int(ci[9]) != 0)		/* Res1: Catch result */
		return (-1);

	return (0);
}

/*
 * Free physical memory (no unmapping is done).
 * This routine is suitable for platforms with 2-cell physical addresses
 * with a single size cell.
 */
void
prom_free_phys(size_t size, unsigned long long physaddr)
{
	cell_t ci[8];
	ihandle_t imemory = prom_memory_ihandle();

	if ((imemory == (ihandle_t)-1))
		return;

	ci[0] = p1275_ptr2cell("call-method");	/* Service name */
	ci[1] = (cell_t)5;			/* #argument cells */
	ci[2] = (cell_t)0;			/* #return cells */
	ci[3] = p1275_ptr2cell("release");	/* Arg1: Method name */
	ci[4] = p1275_ihandle2cell(imemory);	/* Arg2: memory ihandle */
	ci[5] = p1275_size2cell(size);		/* Arg3: SA1: size */
	ci[6] = p1275_ull2cell_high(physaddr);	/* Arg4: SA2: phys.hi */
	ci[7] = p1275_ull2cell_low(physaddr);	/* Arg5: SA3: phys.lo */

	promif_preprom();
	(void) p1275_cif_handler(&ci);
	promif_postprom();
}
