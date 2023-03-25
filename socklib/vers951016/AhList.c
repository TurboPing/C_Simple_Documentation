/* ----------------------------------------------------------------------------
 * AhList.c -- Basic library module for list handling.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: AhList.c,v 1.4 1995/09/25 06:28:21 herren Exp $
 *
 * Status : Release 1.0
 *
 * ----------------------------------------------------------------------------
 * Description
 *
 * This module implements lists as abstract data-types. The following
 * operations on lists are provided:
 *
 * AhListCreate  -- Create a new list.
 * AhListDestroy -- Destroy a list.
 * AhListNelem   -- Get the number of elements in a list.
 * AhListAppend  -- Append an element to a list.
 * AhListPrepend -- Prepend (insert as first) an element to a list.
 * AhListGetTail -- Get the last element of the list.
 * AhListGetHead -- Get the first element of the list.
 *
 * Additionaly iterators on lists are provided. (NOT IMPLEMENTED)
 * An iterator is a view on a list that allows non destructive GetHead
 * operations. Normally a GetHead operation removes the first element from
 * the list, when an iterator is used one can get access to the elements
 * of the list without removing them from the list and therefore to 
 * destroy the lists ordering. The following operations on iterators
 * are provided:
 *
 * AhListIterCreate  -- Create a new iterator on a list.
 * AhListIterDestroy -- Destroy an existing iterator.
 * AhListIterNext    -- Get the next element in the list of the iterator.
 * 
 * We implement secure iterators, this means that the iterators will not
 * have problems with the sequence on the list when elements are added or
 * removed between to iterator operations (AhListIterNext). The following
 * rules apply to an iterator when the list changes:
 *
 * An element is appended to the list:
 *    --> The next call to AhListIterNext will return the next element.
 * An element is prepended to the list:
 *    --> If the iterator was not yet used (no call to AhListIterNext
 *        was issued after AhListIterCreate) the first element is returned.
 *        Otherwise the iterator remains unaffected and the next element
 *        is returned.
 * Whenever the list was emptied:
 *    --> The call to AhListIterNext returns NULL and is destroyed.
 * When the list is destroyed:
 *    --> The call to AhListIterNext returns NULL and is destroyed.
 *
 * The last rule infers a risk of memory leaking. It is the duty of the
 * application to destroy all iterators still in existence whenever it
 * destroys a list, because the AhListDestroy operation will not destroy
 * the iterators themsselves.
 *
 */

static char *AhList_c_rcsid = "$Id: AhList.c,v 1.4 1995/09/25 06:28:21 herren Exp $";

/* ----------------------------------------------------------------------------
 * INCLUDES
 */
#include <stdio.h>
#include "a8620assert.h"
#include "a8620types.h"

/* ----------------------------------------------------------------------------
 * DEFINES
 */
#define AH_LIST_MAGIC	28467482

/* ----------------------------------------------------------------------------
 * TYPES
 */
typedef struct AhListElem {
	Pointer	elem;		/* The element data */
	Pointer	*prev;	/* Link to the previous element. NULL iff head */
	Pointer	*next;	/* Link to the next element. NULL iff tail */
} AhListElem;

typedef struct AhListDesc {
	int	magic;		/* A magic number for not getting fooled */
	int	nelem;		/* The number of elements contained in the list */
	AhListElem	*head;	/* Link to the head of table. NULL iff empty */
	AhListElem	*tail;	/* Link to the tail of table. NULL iff empty */
	Pointer	*iter;	/* A list of iterators. */
} AhListDesc;

typedef AhListDesc	*AhList;

typedef struct AhIterDesc {
	int	magic;		/* A magic number for not getting fooled */
	AhListElem	*curr;
} AhIterDesc;

typedef AhIterDesc	*AhIterator;

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
Public int	AhListErrNo;	/* Error number */
Public char	*AhListErrMsg;	/* Error message */

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 */
Private char	*AhListErrors[2] = {
	"No Error occured.",
	"Could not allocate memory."
};

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/* ----------------------------------------------------------------------------
 * AhListCreate -- Create a new list.
 *
 * Arguments:     None.
 *
 * Returns:       A valid AhList or NULL if an error occured. Sets AhListErrNo
 *                and AhListErrMsg when an error occurs.
 *
 * Side Effects:  Sets AhListErrNo and AhListErrMsg.
 *
 * Algorithm:     Nothing crazy.
 *
 */
Public
AhList
#ifdef UsePrototypes
AhListCreate()
#else
AhListCreate()
#endif
{
	AhList	list;
	
	if ((list = (AhList)calloc(1, sizeof(AhListDesc))) == NULL) {
		AhListErrNo = 1;
		AhListErrMsg = AhListErrors[AhListErrNo];
		return((AhList)NULL);
	}
	
	list->magic = AH_LIST_MAGIC;
	list->nelem = 0;
	list->head = (AhListElem *) NULL;
	list->tail = (AhListElem *) NULL;
	list->iter = (Pointer) NULL;
	
	return(list);
	
} /* End of AhListCreate */

/* ----------------------------------------------------------------------------
 * AhListNelem -- Get the number of elements in a list.
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:       The number of elements remaining in the list. Or -1 if the
 *                list does not exist.
 *
 * Side Effects:  None.
 *
 * Algorithm:     Nothing crazy.
 *
 */
Public
int
#ifdef UsePrototypes
AhListNelem(AhList list)
#else
AhListNelem()
AhList list;
#endif
{

	Wp2("AhListNelem", list != NULL && list->magic == AH_LIST_MAGIC, -1);
	
	return(list->nelem);
	
} /* End of AhListNelem */

/* ----------------------------------------------------------------------------
 * AhListAppend -- Append an element to a list.
 *
 * Arguments:     list -- A valid AhList.
 *                elem -- A pointer to a the element to be appended.
 *
 * Returns:       true when the append was successful, false if an error
 *                occured.
 *
 * Side Effects:  Sets AhListErrNo and AhListErrMsg.
 *
 * Algorithm:     Nothing crazy.
 *
 */
Public
Boolean
#ifdef UsePrototypes
AhListAppend(AhList list, Pointer elem)
#else
AhListAppend()
AhList list;
Pointer elem;
#endif
{
	AhListElem	*lelem;	/* The list element */
	
	Wp2("AhListAppend", list != NULL && list->magic == AH_LIST_MAGIC, false);
	
	if ((lelem = (AhListElem *)calloc(1, sizeof(AhListElem))) == (AhListElem *)NULL) {
		AhListErrNo = 1;
		AhListErrMsg = AhListErrors[AhListErrNo];
		return(false);
	}
	
	lelem->elem = elem;	/* Assign the element to the list element */
	
	/* Handle the links of the list-element */
	
	if (list->nelem == 0) {
		/* First list-element to add */
		list->nelem++;
		list->head = lelem;
		list->tail = lelem;
		lelem->next = (Pointer)NULL;
		lelem->prev = (Pointer)NULL;
	}
	else {
		/* Append the element to the last element */
		list->nelem++;
		list->tail->next = (Pointer)lelem;
		lelem->prev = (Pointer)list->tail;
		list->tail = lelem;
	}
	
	return(true);
	
} /* End of AhListAppend */

/* ----------------------------------------------------------------------------
 * AhListPrepend -- Prepend (insert as first) an element to a list.
 *
 * Arguments:     list -- A valid AhList.
 *                elem -- A pointer to a the element to be prepended.
 *
 * Returns:       true when the prepend was successful, false if an error
 *                occured.
 *
 * Side Effects:  Sets AhListErrNo and AhListErrMsg.
 *
 * Algorithm:     Nothing crazy.
 *
 */
Public
Boolean
#ifdef UsePrototypes
AhListPrepend(AhList list, Pointer elem)
#else
AhListPrepend()
AhList list;
Pointer elem;
#endif
{
	AhListElem	*lelem;
	
	Wp2("AhListPrepend", list != NULL && list->magic == AH_LIST_MAGIC, false);
	
	if ((lelem = (AhListElem *)calloc(1, sizeof(AhListElem))) == (AhListElem *)NULL) {
		AhListErrNo = 1;
		AhListErrMsg = AhListErrors[AhListErrNo];
		return(false);
	}
	
	lelem->elem = elem;	/* Assign the element to the list element */
	
	/* Handle the links of the list-element */

	if(list->nelem == 0) {
		/* First element to add */
		list->nelem++;
		list->head = lelem;
		list->tail = lelem;
		lelem->next = (Pointer)NULL;
		lelem->prev = (Pointer)NULL;
	}
	else {
		/* Prepend the element in front of the first element */
		list->nelem++;
		list->head->prev = (Pointer)lelem;
		lelem->next = (Pointer)list->head;
		list->head = lelem;
	}
	
	return(true);
	
} /* End of AhListPrepend */

/* ----------------------------------------------------------------------------
 * AhListGetTail -- Get the last element of the list.
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:       A pointer to the element of the last element in the 
 *                list. Removes the element from the list.
 *
 * Side Effects:  Sets AhListErrNo and AhListErrMsg.
 *
 * Algorithm:     Nothing crazy.
 *
 */
Public
Pointer
#ifdef UsePrototypes
AhListGetTail(AhList list)
#else
AhListGetTail()
AhList list;
#endif
{
	AhListElem	*lelem;	/* Pointer to the element to be removed */
	Pointer	elem;		/* Pointer to the element's value */
	
	Wp2("AhListGetTail", list != (AhList)NULL
		&& list->magic == AH_LIST_MAGIC && list->nelem != 0, (Pointer)NULL);
	
	lelem = list->tail;
	
	list->nelem--;

	/* Relink the tail */

	if (list->nelem == 0) {
		/* The list was emptied */
		list->head = (AhListElem *)NULL;
		list->tail = (AhListElem *)NULL;
	}
	else {
		/* The list still holds elements */
		list->tail = (AhListElem *)list->tail->prev;
		list->tail->next = (Pointer)NULL;
	}
	
	elem = lelem->elem;
	
	/* Free the memory used by the list-element */
	
	free(lelem);
	
	return(elem);
	
} /* End of AhListGetTail */

/* ----------------------------------------------------------------------------
 * AhListGetHead -- Get the first element of the list.
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:       A pointer to the element of the first element in the 
 *                list. Removes the element from the list.
 *
 * Side Effects:  Sets AhListErrNo and AhListErrMsg.
 *
 * Algorithm:     Nothing crazy.
 *
 */
Public
Pointer
#ifdef UsePrototypes
AhListGetHead(AhList list)
#else
AhListGetHead()
AhList list;
#endif
{
	AhListElem	*lelem;	/* Pointer to the element to be removed */
	Pointer	elem;		/* Pointer to the element's value */
	
	Wp2("AhListGetHead", list != (AhList)NULL
		&& list->magic == AH_LIST_MAGIC && list->nelem != 0, (Pointer)NULL);
	
	lelem = list->head;
	
	list->nelem--;

	/* Relink the head */

	if (list->nelem == 0) {
		/* The list was emptied */
		list->head = (AhListElem *)NULL;
		list->tail = (AhListElem *)NULL;
	}
	else {
		/* The list still holds elements */
		list->head = (AhListElem *)list->head->next;
		list->head->prev = (Pointer)NULL;
	}
	
	elem = lelem->elem;
	
	/* Free the memory used by the list-element */
	
	free(lelem);
	
	return(elem);
	
} /* End of AhListGetHead */

/* ----------------------------------------------------------------------------
 * AhListDestroy -- Destroy a list.
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:       true when the list was successfully destroyed, false if
 *                an error occured.
 *
 * Side Effects:  A call to AhListDestroy on a list still containing elements
 *                will leave the element-data still allocated and will remove
 *                the internal element representation.
 *
 * Algorithm:     Nothing crazy.
 *
 */
Public
Boolean
#ifdef UsePrototypes
AhListDestroy(AhList list)
#else
AhListDestroy()
AhList list;
#endif
{
	Wp2("AhListDestroy", list != (AhList)NULL
				&& list->magic == AH_LIST_MAGIC, false);
	
	if (list->nelem > 0) {
		/* Remove all elements still hanging in the list */
		while(list->nelem != 0 && AhListGetHead(list) != NULL);
	}
	
	free(list);
	
	return(true);
	
} /* End of AhListDestroy */

/* ----------------------------------------------------------------------------
 * AhListIterCreate -- Create a new iterator on a list.
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:       An iterator for the list or NULL if an error occured.
 *
 * Side Effects:  Sets AhListErrNo and AhListErrMsg.
 *
 * Algorithm:     Nothing crazy.
 *
 */
Public
AhIterator
#ifdef UsePrototypes
AhListIterCreate(AhList list)
#else
AhListIterCreate()
AhList list;
#endif
{

	Wp2("AhListIterCreate", list != (AhList)NULL
			&& list->magic == AH_LIST_MAGIC, (AhIterator)NULL);
	
	return((AhIterator)NULL);
	
} /* End of AhListIterCreate */

/* ----------------------------------------------------------------------------
 * AhListIterDestroy -- Destroy an existing iterator.
 *
 * Arguments:
 *
 * Returns:
 *
 * Side Effects:
 *
 * Algorithm:
 *
 */
Public
Boolean
#ifdef UsePrototypes
AhListIterDestroy(AhIterator iter)
#else
AhListIterCreate()
AhIterator iter;
#endif
{

	Wp2("AhListIterNext", iter != NULL && iter->magic == AH_LIST_MAGIC, false);
	
	return(false);
	
} /* End of AhListIterCreate */

/* ----------------------------------------------------------------------------
 * AhListIterNext -- Get the next element in the list of the iterator.
 *
 * Arguments:
 *
 * Returns:
 *
 * Side Effects:
 *
 * Algorithm:
 *
 */
Public
Pointer
#ifdef UsePrototypes
AhListIterNext(AhIterator iter)
#else
AhListIterNext()
#endif
{

	Wp2("AhListIterNext", iter != NULL && iter->magic == AH_LIST_MAGIC, (Pointer)NULL);
	
	return(NULL);

} /* End of AhListIterNext */

/* ----------------------------------------------------------------------------
 * AhListDebugPrint -- Print the contents of the whole list.
 *
 * Arguments:
 *
 * Returns:
 *
 * Side Effects:
 *
 * Algorithm:
 *
 */
Public
Boolean
#ifdef UsePrototypes
AhListDebugPrint(AhList list)
#else
AhListDebugPrint()
#endif
{
	AhListElem	*ptr;
	
	fprintf(stdout, "List Printout\n");
	fprintf(stdout, "-------------\n\n");
	fprintf(stdout, "List Header\n");
	fprintf(stdout, "\tmagic = %d\n", list->magic);
	fprintf(stdout, "\tnelem = %d\n", list->nelem);
	fprintf(stdout, "\thead  = 0x%x\n", list->head);
	fprintf(stdout, "\ttail  = 0x%x\n", list->tail);
	
	ptr = (AhListElem *)list->head;
	
	while(ptr != (AhListElem *)NULL) {
		fprintf(stdout, "List Element (0x%x)\n", ptr);
		fprintf(stdout, "\telem  = 0x%x\n", ptr->elem);
		fprintf(stdout, "\tprev  = 0x%x\n", ptr->prev);
		fprintf(stdout, "\tnext  = 0x%x\n", ptr->next);
		ptr = (AhListElem *)ptr->next;
	}
	
} /* End of AhListDebugPrint */

/* End of AhList.c */
