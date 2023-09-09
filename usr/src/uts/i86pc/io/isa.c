/*
 * Copyright (c) 1990-1999 by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma	ident	"@(#)isa.c	1.28	99/04/26 SMI"

/*
 *	ISA bus nexus driver
 */

#include <sys/types.h>
#include <sys/cmn_err.h>
#include <sys/conf.h>
#include <sys/modctl.h>
#include <sys/autoconf.h>
#include <sys/errno.h>
#include <sys/debug.h>
#include <sys/kmem.h>
#include <sys/ddidmareq.h>
#include <sys/ddi_impldefs.h>
#include <sys/dma_engine.h>
#include <sys/ddi.h>
#include <sys/sunddi.h>
#include <sys/sunndi.h>

/*
 * #define ISA_DEBUG 1
 */

/*
 *      Local data
 */
static ddi_dma_lim_t ISA_dma_limits = {
	0,		/* address low				*/
	0x00ffffff,	/* address high				*/
	0,		/* counter max				*/
	1,		/* burstsize				*/
	DMA_UNIT_8,	/* minimum xfer				*/
	0,		/* dma speed				*/
	(u_int) DMALIM_VER0, /* version				*/
	0x0000ffff,	/* address register			*/
	0x0000ffff,	/* counter register			*/
	1,		/* sector size				*/
	0x00000001,	/* scatter/gather list length		*/
	(u_int) 0xffffffff /* request size			*/
};

static ddi_dma_attr_t ISA_dma_attr = {
	DMA_ATTR_V0,
	(unsigned long long)0,
	(unsigned long long)0x00ffffff,
	0x0000ffff,
	1,
	1,
	1,
	(unsigned long long)0xffffffff,
	(unsigned long long)0x0000ffff,
	1,
	1,
	0
};


/*
 * Config information
 */

static int
isa_dma_allochdl(dev_info_t *, dev_info_t *, ddi_dma_attr_t *,
    int (*waitfp)(caddr_t), caddr_t arg, ddi_dma_handle_t *);

static int
isa_dma_mctl(dev_info_t *, dev_info_t *, ddi_dma_handle_t, enum ddi_dma_ctlops,
    off_t *, size_t *, caddr_t *, u_int);

static int
isa_ctlops(dev_info_t *, dev_info_t *, ddi_ctl_enum_t, void *, void *);

struct bus_ops isa_bus_ops = {
	BUSO_REV,
	i_ddi_bus_map,
	i_ddi_get_intrspec,
	i_ddi_add_intrspec,
	i_ddi_remove_intrspec,
	i_ddi_map_fault,
	ddi_dma_map,
	isa_dma_allochdl,
	ddi_dma_freehdl,
	ddi_dma_bindhdl,
	ddi_dma_unbindhdl,
	ddi_dma_flush,
	ddi_dma_win,
	isa_dma_mctl,
	isa_ctlops,
	ddi_bus_prop_op,
	0,	/* (*bus_get_eventcookie)();	*/
	0,	/* (*bus_add_eventcall)();	*/
	0,	/* (*bus_remove_eventcall)();	*/
	0	/* (*bus_post_event)();		*/
};

static int isa_identify(dev_info_t *devi);
static int isa_probe(dev_info_t *);
static int isa_attach(dev_info_t *devi, ddi_attach_cmd_t cmd);

/*
 * Internal isa ctlops support routines
 */
static int isa_initchild(dev_info_t *child);

struct dev_ops isa_ops = {
	DEVO_REV,		/* devo_rev, */
	0,			/* refcnt  */
	ddi_no_info,		/* info */
	isa_identify,		/* identify */
	isa_probe,		/* probe */
	isa_attach,		/* attach */
	nodev,			/* detach */
	nodev,			/* reset */
	(struct cb_ops *)0,	/* driver operations */
	&isa_bus_ops	/* bus operations */

};

/*
 * Module linkage information for the kernel.
 */

static struct modldrv modldrv = {
	&mod_driverops, /* Type of module.  This is ISA bus driver */
	"isa nexus driver for 'ISA'",
	&isa_ops,	/* driver ops */
};

static struct modlinkage modlinkage = {
	MODREV_1,
	&modldrv,
	NULL
};

int
_init(void)
{
	return (mod_install(&modlinkage));
}

int
_fini(void)
{
	return (mod_remove(&modlinkage));
}

int
_info(struct modinfo *modinfop)
{
	return (mod_info(&modlinkage, modinfop));
}

static int
isa_identify(dev_info_t *devi)
{
	if (strcmp(ddi_get_name(devi), DEVI_ISA_NEXNAME) == 0) {
		return (DDI_IDENTIFIED);
	}
	return (DDI_NOT_IDENTIFIED);
}

static int
isa_probe(register dev_info_t *devi)
{
	int len;
	char bus_type[16];

	len = sizeof (bus_type);
	if (ddi_prop_op(DDI_DEV_T_ANY, devi, PROP_LEN_AND_VAL_BUF, 0,
	    "bus-type", (caddr_t)&bus_type, &len) != DDI_PROP_SUCCESS)
		return (DDI_PROBE_FAILURE);
	if (strcmp(bus_type, DEVI_ISA_NEXNAME))
		return (DDI_PROBE_FAILURE);
	return (DDI_PROBE_SUCCESS);
}

static int
isa_attach(dev_info_t *devi, ddi_attach_cmd_t cmd)
{
	int rval;

	if (cmd != DDI_ATTACH)
		return (DDI_FAILURE);

	if ((rval = i_dmae_init(devi)) == DDI_SUCCESS)
		ddi_report_dev(devi);
	return (rval);
}

static int
isa_dma_allochdl(dev_info_t *dip, dev_info_t *rdip, ddi_dma_attr_t *dma_attr,
    int (*waitfp)(caddr_t), caddr_t arg, ddi_dma_handle_t *handlep)
{
	ddi_dma_attr_merge(dma_attr, &ISA_dma_attr);
	return (ddi_dma_allochdl(dip, rdip, dma_attr, waitfp, arg, handlep));
}

static int
isa_dma_mctl(dev_info_t *dip, dev_info_t *rdip,
    ddi_dma_handle_t handle, enum ddi_dma_ctlops request,
    off_t *offp, size_t *lenp, caddr_t *objp, u_int flags)
{
	int rval;
	ddi_dma_lim_t defalt;

	switch (request) {

	case DDI_DMA_E_PROG:
		return (i_dmae_prog(rdip, (struct ddi_dmae_req *)offp,
		    (ddi_dma_cookie_t *)lenp, (int)objp));

	case DDI_DMA_E_ACQUIRE:
		return (i_dmae_acquire(rdip, (int)objp, (int(*)())offp,
		    (caddr_t)lenp));

	case DDI_DMA_E_FREE:
		return (i_dmae_free(rdip, (int)objp));

	case DDI_DMA_E_STOP:
		i_dmae_stop(rdip, (int)objp);
		return (DDI_SUCCESS);

	case DDI_DMA_E_ENABLE:
		i_dmae_enable(rdip, (int)objp);
		return (DDI_SUCCESS);

	case DDI_DMA_E_DISABLE:
		i_dmae_disable(rdip, (int)objp);
		return (DDI_SUCCESS);

	case DDI_DMA_E_GETCNT:
		i_dmae_get_chan_stat(rdip, (int)objp, (u_long *)0, (int *)lenp);
		return (DDI_SUCCESS);

	case DDI_DMA_E_SWSETUP:
		return (i_dmae_swsetup(rdip, (struct ddi_dmae_req *)offp,
		    (ddi_dma_cookie_t *)lenp, (int)objp));

	case DDI_DMA_E_SWSTART:
		i_dmae_swstart(rdip, (int)objp);
		return (DDI_SUCCESS);

	case DDI_DMA_E_GETLIM:
		bcopy((caddr_t)&ISA_dma_limits, (caddr_t)objp,
		    sizeof (ddi_dma_lim_t));
		return (DDI_SUCCESS);

	case DDI_DMA_E_GETATTR:
		bcopy((caddr_t)&ISA_dma_attr, (caddr_t)objp,
		    sizeof (ddi_dma_attr_t));
		return (DDI_SUCCESS);

	case DDI_DMA_E_1STPTY:
		{
			struct ddi_dmae_req req1stpty =
			    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
			if ((int)objp == 0) {
				req1stpty.der_command = DMAE_CMD_TRAN;
				req1stpty.der_trans = DMAE_TRANS_DMND;
			} else
				req1stpty.der_trans = DMAE_TRANS_CSCD;
			return (i_dmae_prog(rdip, &req1stpty,
			    (ddi_dma_cookie_t *)0, (int)objp));
		}

	case DDI_DMA_IOPB_ALLOC:	/* get contiguous DMA-able memory */
	case DDI_DMA_SMEM_ALLOC:
		if (!offp) {
			defalt = ISA_dma_limits;
			offp = (off_t *)&defalt;
		}
		/*FALLTHROUGH*/
	default:
		rval = ddi_dma_mctl(dip, rdip, handle, request, offp,
		    lenp, objp, flags);
	}
	return (rval);
}

/*
 * Check if driver should be treated as an old pre 2.6 driver
 */
static int
old_driver(dev_info_t *dip)
{
	extern int ignore_hardware_nodes;	/* force flag from ddi_impl.c */

	if (ndi_dev_is_persistent_node(dip)) {
		if (ignore_hardware_nodes)
			return (1);
		if (ddi_getprop(DDI_DEV_T_ANY, dip, DDI_PROP_DONTPASS,
		    "ignore-hardware-nodes", -1) != -1)
			return (1);
	}
	return (0);
}

static struct {
	unsigned long phys_hi;
	unsigned long phys_lo;
	unsigned long size;
} *isa_regs;

/*
 * Return non-zero if device in tree is a PnP isa device
 */
static int
is_pnpisa(dev_info_t *dip)
{
	int proplen, pnpisa;

	if (ndi_dev_is_persistent_node(dip) == 0)
		return (0);
	if (ddi_getlongprop(DDI_DEV_T_ANY, dip, DDI_PROP_DONTPASS, "reg",
		(caddr_t)&isa_regs, &proplen) != DDI_PROP_SUCCESS) {
		return (0);
	}
	pnpisa = isa_regs[0].phys_hi & 0x80000000;
	/*
	 * free the memory allocated by ddi_getlongprop().
	 */
	kmem_free(isa_regs, proplen);
	if (pnpisa)
		return (1);
	else
		return (0);
}

/*ARGSUSED*/
static int
isa_ctlops(dev_info_t *dip, dev_info_t *rdip,
	ddi_ctl_enum_t ctlop, void *arg, void *result)
{
	switch (ctlop) {
	case DDI_CTLOPS_REPORTDEV:
		if (rdip == (dev_info_t *)0)
			return (DDI_FAILURE);
		cmn_err(CE_CONT, "?ISA-device: %s%d\n",
		    ddi_driver_name(rdip), ddi_get_instance(rdip));
		return (DDI_SUCCESS);

	case DDI_CTLOPS_INITCHILD:
		/*
		 * older drivers aren't expecting the "standard" device
		 * node format used by the hardware nodes.  these drivers
		 * only expect their own properties set in their driver.conf
		 * files.  so they tell us not to call them with hardware
		 * nodes by setting the property "ignore-hardware-nodes".
		 */
		if (old_driver((dev_info_t *)arg)) {
			return (DDI_NOT_WELL_FORMED);
		}

		return (isa_initchild((dev_info_t *)arg));

	case DDI_CTLOPS_UNINITCHILD:
		impl_ddi_sunbus_removechild((dev_info_t *)arg);
		return (DDI_SUCCESS);

	case DDI_CTLOPS_SIDDEV:
		/*
		 * All ISA devices need to do confirming probes
		 * unless they are PnP ISA.
		 */
		if (is_pnpisa(dip))
			return (DDI_SUCCESS);
		else
			return (DDI_FAILURE);

	default:
		return (ddi_ctlops(dip, rdip, ctlop, arg, result));
	}
}

static void
isa_vendor(unsigned long id, char *vendor)
{
	vendor[0] = '@' + ((id >> 26) & 0x1f);
	vendor[1] = '@' + ((id >> 21) & 0x1f);
	vendor[2] = '@' + ((id >> 16) & 0x1f);
	vendor[3] = 0;
}

static int
isa_initchild(dev_info_t *child)
{
	char name[80];
	char vendor[8];
	int device, error;
	unsigned long serial;
	int func;
	int proplen;
	int pnpisa = 0;
	int bustype;
	unsigned long base;

	if (ndi_dev_is_persistent_node(child) == 0)
		return (impl_ddi_sunbus_initchild(child));
	if (ddi_getlongprop(DDI_DEV_T_ANY, child,
			DDI_PROP_DONTPASS, "reg", (caddr_t)&isa_regs,
			&proplen) != DDI_PROP_SUCCESS) {
		return (DDI_NOT_WELL_FORMED);
	}
	/*
	 * extract the device identifications
	 */
	pnpisa = isa_regs[0].phys_hi & 0x80000000;
	if (pnpisa) {
		isa_vendor(isa_regs[0].phys_hi, vendor);
		device = isa_regs[0].phys_hi & 0xffff;
		serial = isa_regs[0].phys_lo;
		func = (isa_regs[0].size >> 24) & 0xff;
	} else {
		bustype = isa_regs[0].phys_hi;
		base = isa_regs[0].phys_lo;
	}
	/*
	 * free the memory allocated by ddi_getlongprop().
	 */
	kmem_free(isa_regs, proplen);
	if (pnpisa) {
		if (func != 0)
			(void) sprintf(name, "pnp%s,%04x,%lx,%x",
				vendor, device,
				serial, func);
		else
			(void) sprintf(name, "pnp%s,%04x,%lx",
					vendor, device, serial);
	} else {
		(void) sprintf(name, "%x,%lx", bustype, base);
	}
	error = impl_ddi_sunbus_initchild(child);
	if (error != DDI_SUCCESS)
		return (error);
	ddi_set_name_addr(child, name);
	return (DDI_SUCCESS);
}
