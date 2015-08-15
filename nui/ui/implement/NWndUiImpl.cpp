#include "stdafx.h"
#include "../NWndUi.h"


namespace nui
{
    namespace Ui
    {
        NWndUi::NWndUi()
        {
            realWindow_ = NULL;
        }
        NWndUi::~NWndUi(){}

        NWndUi* NWndUi::GetWndUi(HWND hWnd)
        {
            NWndUi* wndUi = reinterpret_cast<NWndUi*>(::GetProp(hWnd, _T("NWndUiObj")));
            return wndUi;
        }

        void NWndUi::OnWindowChanged(NWindow* window)
        {
            __super::OnWindowChanged(window);
            if(window == NULL || realWindow_ != NULL)
                return;

            Base::NString wndClassName;
            DWORD style;
            DWORD exStyle;
            if(!GetWndData(wndClassName, style, exStyle))
                return;

            Base::NRect rcWnd = GetRootRect();
            HWND hWnd = ::CreateWindowEx(exStyle,
                wndClassName,
                GetText().GetData(),
                WS_TABSTOP | WS_CHILD | (IsVisible() ? WS_VISIBLE : 0) | style,
                rcWnd.Left, rcWnd.Top,
                rcWnd.Width(), rcWnd.Height(),
                window->GetNative(),
                NULL,
                ::GetModuleHandle(NULL),
                0);
            AttachWnd(hWnd);
            ::SendMessage(hWnd, WM_SETFONT, ::SendMessage(window->GetNative(), WM_GETFONT, 0, 0), TRUE);
        }

        void NWndUi::AttachWnd(HWND hWnd)
        {
            NAssertError(hWnd != NULL && ::IsWindow(hWnd), _T("Not A Good Window"));
            if(hWnd == NULL || !::IsWindow(hWnd))
                return;
            NAssertError(realWindow_ == NULL && ::GetProp(hWnd, _T("NWndUiObj")) == NULL, _T("NWndUi Attached Already"));
            realWindow_ = hWnd;
            ::SetProp(realWindow_, _T("NWndUiObj"), reinterpret_cast<HANDLE>(this));
        }
    }
}
