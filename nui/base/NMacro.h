#pragma once


#ifdef _UNICODE
#define __TFILE__   __FILEW__
#else
#define __TFILE__   __FILE__
#endif

#ifndef MERGE_MACRO
#define MERGE_MACRO(x, y) x ## y
#endif  // MERGE_MACRO

#ifndef BEGIN_USE_UNEXPORT_TEMPLATE
#define BEGIN_USE_UNEXPORT_TEMPLATE()   \
    __pragma(warning(push))             \
    __pragma(warning(disable : 4251))
#endif // BEGIN_USE_UNEXPORT_TEMPLATE

#ifndef END_USE_UNEXPORT_TEMPLATE
#define END_USE_UNEXPORT_TEMPLATE()     \
    __pragma(warning(pop))
#endif // END_USE_UNEXPORT_TEMPLATE


#define MemToolParam __FILE__, __LINE__

