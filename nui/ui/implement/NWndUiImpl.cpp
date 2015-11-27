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
        NWndUi::~NWndUi()
        {
            if(realWindow_)
            {
                ::DestroyWindow(realWindow_);
                realWindow_ = NULL;
            }
        }

        NWndUi* NWndUi::GetWndUi(HWND hWnd)
        {
            NWndUi* wndUi = reinterpret_cast<NWndUi*>(::GetProp(hWnd, _T("NWndUiObj")));
            return wndUi;
        }

        bool NWndUi::SetPosImpl(int left, int top, bool force)
        {
            if(!__super::SetPosImpl(left, top, force) || realWindow_ == NULL)
                return false;
            ::SetWindowPos(realWindow_, NULL, left, top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            return true;
        }

        bool NWndUi::SetSizeImpl(int width, int height, bool force)
        {
            if(!__super::SetSizeImpl(width, height, force) || realWindow_ == NULL)
                return false;
            ::SetWindowPos(realWindow_, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
            return false;
        }

        bool NWndUi::IsWndValid() const
        {
            return realWindow_ != NULL && ::IsWindow(realWindow_);
        }

        // NFrame
        // data
        void NWndUi::SetText(const Base::NString& text)
        {
            __super::SetText(text);
            if(IsWndValid())
                ::SetWindowText(realWindow_, __super::GetText().GetData());
        }

        Base::NString NWndUi::GetText() const
        {
            if(IsWndValid())
            {
                TCHAR szText[1000];
                ::GetWindowText(realWindow_, szText, _countof(szText) - 1);
                szText[_countof(szText) - 1] = 0;
                return szText;
            }
            return __super::GetText();
        }

        NText* NWndUi::GetRichText()
        {
            NText* pText = __super::GetRichText();
            if(pText == NULL)
                return NULL;
            pText->SetText(GetText().GetData());
            return pText;
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
