#pragma once


#ifdef NUI_EXPORTS
    #define NUI_API    __declspec(dllexport)
    #define NUI_CLASS  __declspec(dllexport)
    #define NUI_INTF
#else
    #define NUI_API    __declspec(dllimport)
    #define NUI_CLASS  __declspec(dllimport)
    #define NUI_INTF
#endif

#define NSafeRelease(ptr)   for(;;){if(ptr) ptr->Release(); ptr = NULL;break;}


#include <map>
#include <string>
#include <functional>

#include <Unknwn.h>
#include <CommCtrl.h>
#include <GdiPlus.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "GdiPlus.lib")
#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "Urlmon.lib")


#include "./base/DataTypes.h"
#include "./base/noncopyable.h"
#include "./base/NMacro.h"
#include "./base/NMemTool.h"
#include "./base/BaseObj.h"
#include "./base/NAssert.h"
#include "./base/NString.h"
#include "./base/NThread.h"
#include "./base/NReflect.h"
#include "./base/NCore.h"

#include "./data/NBuffer.h"
#include "./data/NDataReader.h"
#include "./data/NFileMapping.h"
#include "./data/NIconv.h"
#include "./data/NModule.h"
#include "./data/NZip.h"

#include "./util/NFileUtil.h"

#ifdef _NOT_USE_NUI_NAMESPACE
using namespace NUI;
using namespace Base;
using namespace Data;
using namespace Util;
#endif  // _NOT_USE_NUI_NAMESPACE
