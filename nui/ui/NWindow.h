#pragma once



#include "../base/BaseObj.h"
#include "NWindowBase.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NWindow : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("window"))
        public:
            NWindow();
            ~NWindow();

            virtual bool Create(HWND parentWindow);
            virtual void Destroy();
            virtual bool DoModal(HWND parentWindow);
            virtual void SetVisible(bool visible);
            virtual void SetPos(int left, int top);
            virtual void SetSize(int width, int height);
            virtual void SetIcon(LPCTSTR iconPath);

        protected:
            bool WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

        protected:
            NWindowBase window_;
        };
    }
}
