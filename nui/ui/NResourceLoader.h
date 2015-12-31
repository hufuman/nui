#pragma once



#include "../base/BaseObj.h"
#include "NImageDraw.h"
#include "NShapeDraw.h"
#include "NTextAttr.h"
#include "NFont.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NResourceLoader : public Base::NBaseObj
        {
        public:
            virtual NImageDraw* LoadImage(LPCTSTR filePath) = 0;
            virtual NShapeDraw* CreateShape(LPCSTR filePath, int line) = 0;
            virtual NTextAttr* CreateText(LPCSTR filePath, int line) = 0;
            virtual NFont* CreateFont(int fontSize, LPCSTR filePath, int line) = 0;
            virtual NFont* CreateFont(LPCSTR filePath, int line) = 0;
        };
    }
}
