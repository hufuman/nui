#pragma once



#include "../base/BaseObj.h"

namespace NUI
{
    namespace UI
    {
        class NUI_INTF NWindow : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("window"))
        public:
            virtual bool Create(HWND parentWindow) = 0;
            virtual void Destroy() = 0;
            virtual bool DoModal(HWND parentWindow) = 0;
            virtual void SetVisible(bool visible) = 0;
            virtual void SetPos(int left, int top) = 0;
            virtual void SetSize(int width, int height) = 0;
        };
    }
}
