/* ----------------------------------------------------------------------------
 * AhList.h -- Basic library module for list handling.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id: AhList.h,v 1.4 1995/09/25 06:28:21 herren Exp $
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
 * ----------------------------------------------------------------------------
 */

#ifndef __AhList_h__

#define __AhList_h__

/* ----------------------------------------------------------------------------
 * INCLUDES
 */
#include "a8620types.h"

/* ----------------------------------------------------------------------------
 * DEFINES
 */

/* ----------------------------------------------------------------------------
 * TYPES
 */
typedef Pointer	AhList;
typedef Pointer	AhIterator;

/* ----------------------------------------------------------------------------
 * GLOBAL VARIABLES
 */
External int	AhListErrNo;	/* Error number */
External char	*AhListErrMsg;	/* Error message */

/* ----------------------------------------------------------------------------
 * LOCAL VARIABLES
 */

/* ----------------------------------------------------------------------------
 * PROCEDURES
 */

/* ----------------------------------------------------------------------------
 * AhListCreate -- Create a new list.
 *
 * Arguments:      None.
 *
 * Returns:        A valid AhList or NULL if an error occured. Sets AhListErrNo
 *                 and AhListErrMsg when an error occurs.
 *
 * Error Messages:   1 -- Could not allocate memory.
 *
 */
External
AhList
#ifdef UsePrototypes
AhListCreate();
#else
AhListCreate();
#endif

/* ----------------------------------------------------------------------------
 * AhListAppend -- Append an element to a list.
 *
 * Arguments:     list -- A valid AhList.
 *                elem -- A pointer to a the element to be appended.
 *
 * Returns:       true when the append was successful, false if an error
 *                occured.
 *
 */
External
Boolean
#ifdef UsePrototypes
AhListAppend(AhList list, Pointer elem);
#else
AhListAppend();
#endif

/* ----------------------------------------------------------------------------
 * AhListPrepend -- Prepend (insert as first) an element to a list.
 *
 * Arguments:     list -- A valid AhList.
 *                elem -- A pointer to a the element to be prepended.
 *
 * Returns:       true when the prepend was successful, false if an error
 *                occured.
 *
 */
External
Boolean
#ifdef UsePrototypes
AhListPrepend(AhList list, Pointer elem);
#else
AhListPrepend();
#endif

/* ----------------------------------------------------------------------------
 * AhListGetTail -- Get the last element of the list.
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:       A pointer to the element of the last element in the 
 *                list. Removes the element from the list.
 *
 */
External
Pointer
#ifdef UsePrototypes
AhListGetTail(AhList list);
#else
AhListGetTail();
#endif

/* ----------------------------------------------------------------------------
 * AhListGetHead -- Get the first element of the list.
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:       A pointer to the element of the first element in the 
 *                list. Removes the element from the list.
 *
 */
External
Pointer
#ifdef UsePrototypes
AhListGetHead(AhList list);
#else
AhListGetHead();
#endif

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
 */
External
Boolean
#ifdef UsePrototypes
AhListDestroy(AhList list);
#else
AhListDestroy();
#endif

/* ----------------------------------------------------------------------------
 * AhListNelem -- Get the number of elements in a list.
 *
 * Arguments:     list -- A valid AhList.
 *
 * Returns:       The number of elements remaining in the list. Or -1 if the
 *                list does not exist.
 *
 */
External
int
#ifdef UsePrototypes
AhListNelem(AhList list);
#else
AhListNelem();
#endif

/* ----------------------------------------------------------------------------
 * AhListIterCreate -- Create a new iterator on a list.
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
External
AhIterator
#ifdef UsePrototypes
AhListIterCreate(AhList list);
#else
AhListIterCreate();
#endif

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
External
Boolean
#ifdef UsePrototypes
AhListIterDestroy(AhIterator iter);
#else
AhListIterDestroy();
#endif

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
External
Pointer
#ifdef UsePrototypes
AhListIterNext(AhIterator iter);
#else
AhListIterNext();
#endif

#endif __AhList_h__
