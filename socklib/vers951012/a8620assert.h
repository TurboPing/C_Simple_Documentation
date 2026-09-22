/* ----------------------------------------------------------------------------
 * a8620assert.h -- Assertions for a8620.
 * ----------------------------------------------------------------------------
 *
 * (c) 1995 Alcatel STR, Zuerich, Switzerland.
 *
 * ----------------------------------------------------------------------------
 *
 * $Id$
 *
 * Description
 *
 * We define some macros for the use of assertions in our programs.
 * These macros will print a string to the standard-error iff the expression
 * which defines the assertion fails and exit the program with an exit code of
 * 1.
 *
 * We support the following assertions:
 *
 * Assert(ex) -- Generic assertion over an expression ex.
 *
 * Wp(func, ex)       -- Weakest precondition with expression ex in the function
 *                       with name func.
 *
 * PreCond(ex)        -- Assert with expression ex used as precondition.
 *
 * PostCond(ex)       -- Assert with expression ex used as postcondition.
 *
 * LoopInv(ex)        -- Assert with expression ex used as loop invariant.
 *
 * One can trigger the use of the assertions by defining the preprocessor
 * variable NESASSERT before the inclusion of this include file.
 *
 * ----------------------------------------------------------------------------
 */


#ifndef __a8620assert_h__

#define __a8620assert_h__

static char *a8620assert_h_rcsid = "$Id$";

/* ----------------------------------------------------------------------------
 * INCLUDES
 */

/* ----------------------------------------------------------------------------
 * DEFINES
 */

#ifndef NESASSERT

#include	<stdio.h>

#ifdef __STDC__
# define	_EsAssert(what, ex)	{if (!(ex)){(void)fprintf(stderr, "%s failed: '%s' file \"%s\", line %d\n", what, #ex, __FILE__, __LINE__);exit(1);}}
# define	_EsAssert2(what, ex, ret)	{if (!(ex)){(void)fprintf(stderr, "%s failed: '%s' file \"%s\", line %d\n", what, #ex, __FILE__, __LINE__); return(ret);}}
# define	_EsAssert3(what, ex)	{if (!(ex)){(void)fprintf(stderr, "%s failed: '%s' file \"%s\", line %d\n", what, #ex, __FILE__, __LINE__); return;}}
#else
# define	_EsAssert(what, ex)	{if (!(ex)){(void)fprintf(stderr, "%s failed: file \"%s\", line %d\n", what, __FILE__, __LINE__);exit(1);}}
# define	_EsAssert2(what, ex, ret)	{if (!(ex)){(void)fprintf(stderr, "%s failed: file \"%s\", line %d\n", what, __FILE__, __LINE__);return(ret);}}
# define	_EsAssert3(what, ex)	{if (!(ex)){(void)fprintf(stderr, "%s failed: file \"%s\", line %d\n", what, __FILE__, __LINE__);return;}}
# define	_EsWp(what, func, ex)	{if (!(ex)){(void)fprintf(stderr, "%s %s failed: file \"%s\", line %d\n", what, func,  __FILE__, __LINE__);exit(1);}}
# define	_EsWp2(what, func, ex, ret)	{if (!(ex)){(void)fprintf(stderr, "%s %s failed: file \"%s\", line %d\n", what, func,  __FILE__, __LINE__);return(ret);}}
# define	_EsWp3(what, func, ex)	{if (!(ex)){(void)fprintf(stderr, "%s %s failed: file \"%s\", line %d\n", what, func,  __FILE__, __LINE__);return;}}
#endif __STDC__

#else 

#ifdef __STDC__
# define	_EsAssert(what, ex)	{if (!(ex)){(void)fprintf(stderr,"%s failed: '%s' file \"%s\", line %d\n", what, #ex, __FILE__, __LINE__);exit(1);}}
# define	_EsAssert2(what, ex, ret)	{if (!(ex)){(void)fprintf(stderr,"%s failed: '%s' file \"%s\", line %d\n", what, #ex, __FILE__, __LINE__); return(ret);}}
# define	_EsAssert3(what, ex)	{if (!(ex)){(void)fprintf(stderr,"%s failed: '%s' file \"%s\", line %d\n", what, #ex, __FILE__, __LINE__); return;}}
#else
# define	_EsAssert(what, ex)	{if (!(ex)){(void)fprintf(stderr,"%s failed: file \"%s\", line %d\n", what, __FILE__, __LINE__);exit(1);}}
# define	_EsAssert2(what, ex, ret)	{if (!(ex)){(void)fprintf(stderr,"%s failed: file \"%s\", line %d\n", what, __FILE__, __LINE__);return(ret);}}
# define	_EsAssert3(what, ex)	{if (!(ex)){(void)fprintf(stderr,"%s failed: file \"%s\", line %d\n", what, __FILE__, __LINE__);return;}}
# define	_EsWp(what, func, ex)	{if (!(ex)){(void)fprintf(stderr,"%s %s failed: file \"%s\", line %d\n", what, func,  __FILE__, __LINE__);exit(1);}}
# define	_EsWp2(what, func, ex, ret)	{if (!(ex)){(void)fprintf(stderr,"%s %s failed: file \"%s\", line %d\n", what, func,  __FILE__, __LINE__);return(ret);}}
# define	_EsWp3(what, func, ex)	{if (!(ex)){(void)fprintf(stderr,"%s %s failed: file \"%s\", line %d\n", what, func,  __FILE__, __LINE__);return;}}
#endif __STDC__

#endif NESASSERT

#ifndef NESASSERT

#ifdef __STDC__
# define	Assert(ex)		_EsAssert("Assertion", ex)
# define	Wp(func, ex)	_EsAssert("Weakest Precondition for " #func, ex)
# define	PreCond(ex)		_EsAssert("Precondition", ex)
# define	PostCond(ex)	_EsAssert("Postcondition", ex)
# define	LoopInv(ex)		_EsAssert("Loop Invariant", ex)
# define	Assert2(ex, ret)		_EsAssert2("Assertion", ex, ret)
# define	Wp2(func, ex, ret)	_EsAssert2("Weakest Precondition for " #func, ex, ret)
# define	PreCond2(ex, ret)		_EsAssert2("Precondition", ex, ret)
# define	PostCond2(ex, ret)	_EsAssert2("Postcondition", ex, ret)
# define	LoopInv2(ex, ret)		_EsAssert2("Loop Invariant", ex, ret)
# define	Assert3(ex)		_EsAssert3("Assertion", ex)
# define	Wp3(func, ex)	_EsAssert3("Weakest Precondition for " #func, ex)
# define	PreCond3(ex)		_EsAssert3("Precondition", ex)
# define	PostCond3(ex)	_EsAssert3("Postcondition", ex)
# define	LoopInv3(ex)		_EsAssert3("Loop Invariant", ex)
#else
# define	Assert(ex)		_EsAssert("Assertion", (ex))
# define	Wp(func, ex)	_EsWp("Weakest Precondition for ", func, (ex))
# define	PreCond(ex)		_EsAssert("Precondition", (ex))
# define	PostCond(ex)	_EsAssert("Postcondition", (ex))
# define	LoopInv(ex)		_EsAssert("Loop Invariant",(ex))
# define	Assert2(ex, ret)		_EsAssert2("Assertion", (ex), ret)
# define	Wp2(func, ex, ret)	_EsWp2("Weakest Precondition for ", func, (ex), ret)
# define	PreCond2(ex, ret)		_EsAssert2("Precondition", (ex), ret)
# define	PostCond2(ex, ret)	_EsAssert2("Postcondition", (ex), ret)
# define	LoopInv2(ex, ret)		_EsAssert2("Loop Invariant",(ex), ret)
# define	Assert3(ex)		_EsAssert3("Assertion", (ex))
# define	Wp3(func, ex)	_EsWp3("Weakest Precondition for ", func, (ex))
# define	PreCond3(ex)		_EsAssert3("Precondition", (ex))
# define	PostCond3(ex)	_EsAssert3("Postcondition", (ex))
# define	LoopInv3(ex)		_EsAssert3("Loop Invariant",(ex))
#endif


# else

#define	_EsAssert(ex)
#define	Assert(ex)
#define	Wp(func, ex)
#define	PreCond(ex)
#define	PostCond(ex)
#define	LoopInv(ex)

# endif NESASSERT

/* ----------------------------------------------------------------------------
 * TYPES
 */

#endif __a8620assert_h__
