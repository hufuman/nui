// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <tchar.h>
#include <windows.h>

#include <Unknwn.h>
#include <GdiPlus.h>


#pragma warning(disable: 4996)
#include "nui.h"


#pragma comment(lib, "zip.lib")
#pragma comment(lib, "tinyxml.lib")

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "GdiPlus.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "Urlmon.lib")
