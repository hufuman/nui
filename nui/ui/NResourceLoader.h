#pragma once



#include "../base/BaseObj.h"
#include "NImageDraw.h"
#include "NShapeDraw.h"
#include "NText.h"
#include "NFont.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NResourceLoader : public Base::NBaseObj
        {
        public:
            virtual NImageDraw* LoadImage(LPCTSTR filePath, bool& hasExtInfo) = 0;
            virtual NShapeDraw* CreateShape(LPCSTR filePath, int line) = 0;
            virtual NText* CreateText(LPCTSTR text, LPCSTR filePath, int line) = 0;
            virtual NFont* CreateFont(int fontSize, LPCSTR filePath, int line) = 0;
        };
    }
}
