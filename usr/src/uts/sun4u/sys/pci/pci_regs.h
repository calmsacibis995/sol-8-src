/*
 * Copyright (c) 1991-1998, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#ifndef _SYS_PCI_REGS_H
#define	_SYS_PCI_REGS_H

#pragma ident	"@(#)pci_regs.h	1.30	99/06/14 SMI"

#ifdef	__cplusplus
extern "C" {
#endif

#define	PSYCHO_PERF_PCR_OFF			0x00000100
#define	PSYCHO_PERF_PIC_OFF			0x00000108

/*
 * Offsets of registers in the interrupt block:
 */

#define	COMMON_IB_SLOT_INTR_MAP_REG_OFFSET	0x00000C00
#define	COMMON_IB_OBIO_INTR_MAP_REG_OFFSET	0x00001000
#define	COMMON_IB_UPA0_INTR_MAP_REG_OFFSET	0x00006000
#define	COMMON_IB_UPA1_INTR_MAP_REG_OFFSET	0x00008000
#define	COMMON_IB_SLOT_INTR_STATE_DIAG_REG	0x0000A800
#define	COMMON_IB_OBIO_INTR_STATE_DIAG_REG	0x0000A808
#define	COMMON_IB_SLOT_CLEAR_INTR_REG_OFFSET	0x00001400
#define	COMMON_IB_INTR_RETRY_TIMER_OFFSET	0x00001A00

#define	SCHIZO_IB_SLOT_INTR_MAP_REG_OFFSET	0x00001100
#define	SCHIZO_IB_INTR_MAP_REG_OFFSET		0x00001000
#define	SCHIZO_IB_CLEAR_INTR_REG_OFFSET		0x00001400

/*
 * Offsets of registers in the ECC block:
 */
#define	COMMON_ECC_CNTRL_REG_OFFSET		0x00000020
#define	COMMON_UE_ASYNC_FLT_STATUS_REG_OFFSET	0x00000030
#define	COMMON_UE_ASYNC_FLT_ADDR_REG_OFFSET	0x00000038
#define	COMMON_CE_ASYNC_FLT_STATUS_REG_OFFSET	0x00000040
#define	COMMON_CE_ASYNC_FLT_ADDR_REG_OFFSET	0x00000048

/*
 * Offsets of registers in the iommu block:
 */
#define	COMMON_IOMMU_CTRL_REG_OFFSET		0x00000200
#define	COMMON_IOMMU_TSB_BASE_ADDR_REG_OFFSET	0x00000208
#define	COMMON_IOMMU_FLUSH_PAGE_REG_OFFSET	0x00000210

/* Sabre specific */
#define	DMA_WRITE_SYNC_REG			0x00001C20

#define	COMMON_IOMMU_TLB_TAG_DIAG_ACC_OFFSET	0x0000A580
#define	COMMON_IOMMU_TLB_DATA_DIAG_ACC_OFFSET	0x0000A600

/*
 * (psycho and schizo) control register bit definitions:
 */
#define	COMMON_CB_CONTROL_STATUS_IGN		0x0007c00000000000ull
#define	COMMON_CB_CONTROL_STATUS_IGN_SHIFT	46
#define	COMMON_CB_CONTROL_STATUS_APCKEN		0x0000000000000008ull
#define	COMMON_CB_CONTROL_STATUS_APERR		0x0000000000000004ull
#define	COMMON_CB_CONTROL_STATUS_IAP		0x0000000000000002ull

/*
 * (psycho and schizo) interrupt mapping register bit definitions:
 */
#define	COMMON_INTR_MAP_REG_VALID		0x0000000080000000ull
#define	COMMON_INTR_MAP_REG_TID			0x000000007C000000ull
#define	COMMON_INTR_MAP_REG_IGN			0x00000000000007C0ull
#define	COMMON_INTR_MAP_REG_INO			0x000000000000003full
#define	COMMON_INTR_MAP_REG_TID_SHIFT		26
#define	COMMON_INTR_MAP_REG_IGN_SHIFT		6

/*
 * psycho clear interrupt register bit definitions:
 */
#define	COMMON_CLEAR_INTR_REG_MASK		0x0000000000000003ull
#define	COMMON_CLEAR_INTR_REG_IDLE		0x0000000000000000ull
#define	COMMON_CLEAR_INTR_REG_RECEIVED		0x0000000000000001ull
#define	COMMON_CLEAR_INTR_REG_RSVD		0x0000000000000002ull
#define	COMMON_CLEAR_INTR_REG_PENDING		0x0000000000000003ull

/*
 * psycho and schizo ECC control register bit definitions:
 */
#define	COMMON_ECC_CTRL_ECC_EN			0x8000000000000000ull
#define	COMMON_ECC_CTRL_UE_INTEN		0x4000000000000000ull
#define	COMMON_ECC_CTRL_CE_INTEN		0x2000000000000000ull

/*
 * sabre ECC UE AFSR bit definitions:
 */
#define	SABRE_UE_AFSR_SDTE_SHIFT		57
#define	SABRE_UE_AFSR_PDTE_SHIFT		56
#define	SABRE_UE_ARSR_DTE_MASK			0x0000000000000003ull
#define	SABRE_UE_AFSR_E_SDTE			0x2
#define	SABRE_UE_AFSR_E_PDTE			0x1

/*
 * psycho and schizo ECC UE AFSR bit definitions:
 */
#define	COMMON_ECC_UE_AFSR_PE_SHIFT		61
#define	COMMON_ECC_UE_AFSR_SE_SHIFT		58
#define	COMMON_ECC_UE_AFSR_E_MASK		0x0000000000000007ull
#define	COMMON_ECC_UE_AFSR_E_PIO		0x0000000000000004ull
#define	COMMON_ECC_UE_AFSR_E_DRD		0x0000000000000002ull
#define	COMMON_ECC_UE_AFSR_E_DWR		0x0000000000000001ull

/*
 * psycho and schizo ECC CE AFSR bit definitions:
 */
#define	COMMON_ECC_CE_AFSR_PE_SHIFT		61
#define	COMMON_ECC_CE_AFSR_SE_SHIFT		58
#define	COMMON_ECC_CE_AFSR_E_MASK		0x0000000000000007ull
#define	COMMON_ECC_CE_AFSR_E_PIO		0x0000000000000004ull
#define	COMMON_ECC_CE_AFSR_E_DRD		0x0000000000000002ull
#define	COMMON_ECC_CE_AFSR_E_DWR		0x0000000000000001ull

/*
 * psycho and schizo pci control register bits:
 */
#define	COMMON_PCI_CTRL_SBH_ERR			0x0000000800000000ull
#define	COMMON_PCI_CTRL_SERR			0x0000000400000000ull
#define	COMMON_PCI_CTRL_SPEED			0x0000000200000000ull

/*
 * psycho and schizo PCI diagnostic register bit definitions:
 */
#define	COMMON_PCI_DIAG_DIS_RETRY		0x0000000000000040ull
#define	COMMON_PCI_DIAG_DIS_INTSYNC		0x0000000000000020ull

/*
 * psycho and schizo IOMMU control register bit definitions:
 */
#define	COMMON_IOMMU_CTRL_ENABLE	0x0000000000000001ull
#define	COMMON_IOMMU_CTRL_DIAG_ENABLE	0x0000000000000002ull
#define	COMMON_IOMMU_CTRL_TSB_SZ_SHIFT	16
#define	COMMON_IOMMU_CTRL_TBW_SZ_SHIFT	2
#define	COMMON_IOMMU_CTRL_LCK_ENABLE	0x0000000000800000ull

/*
 * psycho and schizo streaming cache control register bit definitions:
 */
#define	COMMON_SC_CTRL_ENABLE		0x0000000000000001ull
#define	COMMON_SC_CTRL_DIAG_ENABLE	0x0000000000000002ull
#define	COMMON_SC_CTRL_RR__DISABLE	0x0000000000000004ull
#define	COMMON_SC_CTRL_LRU_LE		0x0000000000000008ull

/*
 * offsets of PCI address spaces from base address:
 */
#define	PCI_CONFIG			0x001000000ull
#define	PCI_A_IO			0x002000000ull
#define	PCI_B_IO			0x002010000ull
#define	PCI_A_MEMORY			0x100000000ull
#define	PCI_B_MEMORY			0x180000000ull
#define	PCI_IO_SIZE			0x000010000ull
#define	PCI_MEM_SIZE			0x080000000ull

#ifdef	__cplusplus
}
#endif

#endif	/* _SYS_PCI_REGS_H */