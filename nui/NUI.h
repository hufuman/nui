#pragma once


#ifdef NUI_EXPORTS
    #define NUI_API         __declspec(dllexport)
    #define NUI_CLASS       __declspec(dllexport)
    #define NUI_INTF
    #define NUI_TEMPLATE
#else
    #define NUI_API         __declspec(dllimport)
    #define NUI_CLASS       __declspec(dllimport)
    #define NUI_INTF
    #define NUI_TEMPLATE
#endif

#define NSafeRelease(ptr)   for(;;){if(ptr) ptr->Release(); ptr = NULL;break;}
#define NEnsureRelease(ptr)   for(;ptr;){if(ptr->Release() == 0) {ptr = NULL;break;}}


#include <map>
#include <string>
#include <functional>

#include <CommCtrl.h>
#include <shellapi.h>


#include "./base/noncopyable.h"
#include "./base/NMacro.h"
#include "./base/NAssert.h"
#include "./base/NAutoPtr.h"
#include "./base/NInstPtr.h"
#include "./base/FastDelegate.h"
using namespace fastdelegate;
#include "./base/DataTypes.h"
#include "./base/NMemTool.h"
#include "./base/BaseObj.h"
#include "./base/NString.h"
#include "./base/NThread.h"
#include "./base/NReflect.h"
#include "./base/NCore.h"
#include "./base/NHolder.h"

#include "./data/NBuffer.h"
#include "./data/NDataReader.h"
#include "./data/NFileMapping.h"
#include "./data/NIconv.h"
#include "./data/NModule.h"
#include "./data/NZip.h"
#include "./data/NListT.h"
#include "./data/NArrayT.h"
#include "./data/NFileSystem.h"

#include "./util/NFileUtil.h"

#include "./ui/NRenderDef.h"
#include "./ui/NMsgLoop.h"
#include "./ui/NWindowBase.h"
#include "./ui/NTimerSrv.h"
#include "./ui/NEvent.h"
#include "./ui/NFrame.h"
#include "./ui/NImage.h"
#include "./ui/NResourceLoader.h"
#include "./ui/NUiBus.h"
#include "./ui/NRender.h"
#include "./ui/NWindow.h"

#ifdef _NOT_USE_NUI_NAMESPACE
using namespace nui;
using namespace Base;
using namespace Data;
using namespace Util;
using namespace Ui;
#endif  // _NOT_USE_NUI_NAMESPACE

