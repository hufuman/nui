#pragma once


#ifdef _UNICODE
#define __TFILE__   __FILEW__
#else
#define __TFILE__   __FILE__
#endif

#ifndef MERGE_MACRO
#define MERGE_MACRO(x, y) x ## y
#endif  // MERGE_MACRO
