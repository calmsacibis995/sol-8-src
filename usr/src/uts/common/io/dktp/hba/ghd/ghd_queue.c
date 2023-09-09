/*
 * Copyright (c) 1999, by Sun Microsystems, Inc.
 * All rights reserved.
 */

#pragma	ident	"@(#)ghd_queue.c	1.6	99/04/09 SMI"


#include <sys/types.h>
#include <sys/param.h>
#include <sys/debug.h>
#include "ghd_queue.h"



void
L1_add(L1_t *lp, L1el_t *lep, void *datap)
{
	/* init the list element */
	lep->le_nextp = NULL;
	lep->le_datap = datap;

	if (!lp->l1_tailp) {
		/* list is empty */
		lp->l1_headp = lep;
	} else {
		/* add it to the tailend */
		lp->l1_tailp->le_nextp = lep;
	}

	lp->l1_tailp = lep;
}


/*
 * L1Delete()
 *
 *	Remove a specific entry from a singly-linked list.
 *
 */

void
L1_delete(L1_t *lp, L1el_t *lep)
{
	L1el_t	*prevp;

	if (lp->l1_headp == lep) {
		/* it's the first entry in the list */
		if ((lp->l1_headp = lep->le_nextp) == NULL) {
			/* the list is now empty */
			lp->l1_tailp = NULL;
		}
		return;
	}

	for (prevp = lp->l1_headp; prevp != NULL; prevp = prevp->le_nextp) {
		if (prevp->le_nextp == lep) {
			if ((prevp->le_nextp = lep->le_nextp) == NULL)
				lp->l1_tailp = prevp;
			return;
		}
	}
	/* its not on this list */
}


/*
 * L1_remove()
 *
 *	Remove the entry at the head of the list (if any).
 *
 */

void *
L1_remove(L1_t *lp)
{
	L1el_t	*lep;

	/* pop the first one off the list head */
	if ((lep = lp->l1_headp) == NULL) {
		return (NULL);
	}

	/* if the list is now empty fix the tail pointer */
	if ((lp->l1_headp = lep->le_nextp) == NULL)
		lp->l1_tailp = NULL;

	lep->le_nextp = NULL;

	return (lep->le_datap);
}


void
L2_add(L2el_t *headp, L2el_t *elementp, void *private)
{

	ASSERT(headp != NULL && elementp != NULL);
	ASSERT(headp->l2_nextp != NULL);
	ASSERT(headp->l2_prevp != NULL);

	elementp->l2_private = private;

	elementp->l2_nextp = headp;
	elementp->l2_prevp = headp->l2_prevp;
	headp->l2_prevp->l2_nextp = elementp;
	headp->l2_prevp = elementp;
}

void
L2_delete(L2el_t *elementp)
{

	ASSERT(elementp != NULL);
	ASSERT(elementp->l2_nextp != NULL);
	ASSERT(elementp->l2_prevp != NULL);
	ASSERT(elementp->l2_nextp->l2_prevp == elementp);
	ASSERT(elementp->l2_prevp->l2_nextp == elementp);

	elementp->l2_prevp->l2_nextp = elementp->l2_nextp;
	elementp->l2_nextp->l2_prevp = elementp->l2_prevp;

	/* link it to itself in case someone does a double delete */
	elementp->l2_nextp = elementp;
	elementp->l2_prevp = elementp;
}


void
L2_add_head(L2el_t *headp, L2el_t *elementp, void *private)
{

	ASSERT(headp != NULL && elementp != NULL);
	ASSERT(headp->l2_nextp != NULL);
	ASSERT(headp->l2_prevp != NULL);

	elementp->l2_private = private;

	elementp->l2_prevp = headp;
	elementp->l2_nextp = headp->l2_nextp;
	headp->l2_nextp->l2_prevp = elementp;
	headp->l2_nextp = elementp;
}



/*
 * L2_remove()
 *
 *	Remove the entry from the head of the list (if any).
 *
 */

void *
L2_remove_head(L2el_t *headp)
{
	L2el_t *elementp;

	ASSERT(headp != NULL);

	if (L2_EMPTY(headp))
		return (NULL);

	elementp = headp->l2_nextp;

	headp->l2_nextp = elementp->l2_nextp;
	elementp->l2_nextp->l2_prevp = headp;

	/* link it to itself in case someone does a double delete */
	elementp->l2_nextp = elementp;
	elementp->l2_prevp = elementp;

	return (elementp->l2_private);
}

void *
L2_next(L2el_t *elementp)
{

	ASSERT(elementp != NULL);

	if (L2_EMPTY(elementp))
		return (NULL);
	return (elementp->l2_nextp->l2_private);
}