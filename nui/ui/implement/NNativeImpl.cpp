#include "stdafx.h"
#include "../NNative.h"


namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NNative);

        NNative::NNative()
        {
            realWindow_ = NULL;
        }
        NNative::~NNative()
        {
            if(realWindow_)
            {
                ::DestroyWindow(realWindow_);
                realWindow_ = NULL;
            }
        }

        void NNative::Attach(NFrame* parentFrame, HWND hwndNative)
        {
            NAssertError(hwndNative && ::IsWindow(hwndNative), _T("Invalid window handle"));

            Create(parentFrame);
            AttachWnd(hwndNative);
        }

        HWND NNative::GetNative() const
        {
            return realWindow_;
        }

        NNative* NNative::GetWndUi(HWND hWnd)
        {
            NNative* wndUi = reinterpret_cast<NNative*>(::GetProp(hWnd, _T("NNativeObj")));
            return wndUi;
        }

        void NNative::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:native"));
#endif  // _NO_NUI_PARSER_
        }

        void NNative::OnSizeChanged(int width, int height)
        {
            __super::OnSizeChanged(width, height);
            if(realWindow_)
            {
                ::SetWindowPos(realWindow_, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME);
            }
        }

        void NNative::OnPosChanged(int left, int top)
        {
            __super::OnPosChanged(left, top);
            if(realWindow_)
            {
                Base::NRect rcNative = GetRootRect();
                ::SetWindowPos(realWindow_, NULL, rcNative.Left, rcNative.Top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_DRAWFRAME);
            }
        }

        bool NNative::IsWndValid() const
        {
            return realWindow_ != NULL && ::IsWindow(realWindow_);
        }

        bool NNative::GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle)
        {
            wndClassName = _T("");
            style = WS_TABSTOP | WS_CHILD;
            exStyle = 0;
            return false;
        }

        bool NNative::OnParentCommand(WORD notifyCode)
        {
            UNREFERENCED_PARAMETER(notifyCode);
            return false;
        }

        // NFrame
        // data
        void NNative::SetText(const Base::NString& text)
        {
            __super::SetText(text);
            if(IsWndValid())
                ::SetWindowText(realWindow_, __super::GetText().GetData());
        }

        Base::NString NNative::GetText() const
        {
            Base::NString text;
            if(IsWndValid())
            {
                TCHAR fixedBuffer[1024];
                TCHAR* buffer = fixedBuffer;
                int textLength = ::GetWindowTextLength(realWindow_) + 1;
                if(textLength > _countof(fixedBuffer))
                    buffer = NNewArray(TCHAR, textLength);
                textLength = ::GetWindowText(realWindow_, buffer, textLength);
                buffer[textLength] = 0;
                text = buffer;
                if(buffer != fixedBuffer)
                    NDeleteArray(buffer);
                return text;
            }
            else
            {
                text = __super::GetText();
            }
            return text;
        }

        NText* NNative::GetRichText()
        {
            NText* pText = __super::GetRichText();
            if(pText == NULL)
                return NULL;
            pText->SetText(GetText().GetData());
            return pText;
        }

        // flags
        bool NNative::SetVisible(bool visible)
        {
            if(!__super::SetVisible(visible) || realWindow_ == NULL)
                return false;
            ::ShowWindow(realWindow_, IsVisible() ? SW_SHOW : SW_HIDE);
            return true;
        }

        bool NNative::SetEnabled(bool enabled)
        {
            if(!__super::SetEnabled(enabled) || realWindow_ == NULL)
                return false;
            ::EnableWindow(realWindow_, IsEnabled());
            return true;
        }

        Base::NSize NNative::GetAutoSize() const
        {
            Base::NSize autoSize = __super::GetAutoSize();
            autoSize.Width = max(autoSize.Width, 128);
            autoSize.Height = max(autoSize.Height, 20);
            return autoSize;
        }

        void NNative::OnWindowChanged(NWindow* window)
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

        void NNative::AttachWnd(HWND hWnd)
        {
            NAssertError(hWnd != NULL && ::IsWindow(hWnd), _T("Not A Good Window"));
            if(hWnd == NULL || !::IsWindow(hWnd))
                return;
            NAssertError(realWindow_ == NULL && ::GetProp(hWnd, _T("NNativeObj")) == NULL, _T("NNative Attached Already"));
            realWindow_ = hWnd;
            ::SetProp(realWindow_, _T("NNativeObj"), reinterpret_cast<HANDLE>(this));

            RECT rcWnd;
            ::GetWindowRect(realWindow_, &rcWnd);
            ::OffsetRect(&rcWnd, -rcWnd.left, -rcWnd.top);
            ::MapWindowRect(realWindow_, ::GetParent(realWindow_), &rcWnd);

            SetPos(rcWnd.left, rcWnd.top);
            SetSize(rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);
        }
    }
}
