// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <Unknwn.h>
#include <GdiPlus.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "GdiPlus.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "Urlmon.lib")


#define _NOT_USE_NUI_NAMESPACE 1
#define _NO_NUI_PARSER_ 1
#include "../NUI/NUI.h"
// TODO: reference additional headers your program requires here
