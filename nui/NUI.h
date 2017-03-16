#pragma once


#if defined(NUI_STATIC)
    #define NUI_API
    #define NUI_CLASS
    #define NUI_INTF
    #define NUI_TEMPLATE
#elif defined(NUI_EXPORTS)
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

#include <map>
#include <list>
#include <string>
#include <algorithm>
#include <functional>

#include <CommCtrl.h>
#include <CommDlg.h>
#include <shellapi.h>
#include <windowsx.h>

#pragma warning(push)
#pragma warning(disable: 4996)

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
#include "./data/NStringBundle.h"

#include "./util/NShellUtil.h"
#include "./util/NFileUtil.h"
#include "./util/NMisc.h"
#include "./util/NDiagnose.h"

#include "./ui/UiDef.h"
#include "./ui/NCursor.h"
#include "./ui/NRenderDef.h"
#include "./ui/NMsgLoop.h"
#include "./ui/NEvent.h"
#include "./ui/NWindowBase.h"
#include "./ui/NTimerSrv.h"
#include "./ui/NUiProc.h"
#include "./ui/NFrameBase.h"
#include "./ui/NFrame.h"
#include "./ui/NButton.h"
#include "./ui/NCheckBox.h"
#include "./ui/NRadioBox.h"
#include "./ui/NHeader.h"
#include "./ui/NResourceLoader.h"
#include "./ui/NUiBus.h"
#include "./ui/NRender.h"
#include "./ui/NDraw.h"
#include "./ui/NWindow.h"
#include "./ui/NFont.h"
#include "./ui/NLabel.h"
#include "./ui/NLink.h"
#include "./ui/NImageDraw.h"
#include "./ui/NImage.h"
#include "./ui/NShapeDraw.h"
#include "./ui/NTextAttr.h"
#include "./ui/NScroll.h"
#include "./ui/NProgress.h"
#include "./ui/NLayout.h"
#include "./ui/NList.h"
#include "./ui/NNative.h"
#include "./ui/NEdit.h"

#include "./parser/NParser.h"

#pragma warning(pop)


#ifdef _NOT_USE_NUI_NAMESPACE
using namespace nui;
using namespace Base;
using namespace Data;
using namespace Util;
using namespace Ui;
using namespace Parser;
#endif  // _NOT_USE_NUI_NAMESPACE

