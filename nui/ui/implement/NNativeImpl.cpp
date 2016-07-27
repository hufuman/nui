#include "stdafx.h"
#include "../NNative.h"


#include "./Gdi/GdiObjMgr.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NNative);

        NNative::NNative()
        {
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagCanHover, true);

            Base::NAutoPtr<Ui::NShapeDraw> bkgDraw = Ui::NUiBus::Instance().GetResourceLoader()->CreateShape(MemToolParam);
            bkgDraw->SetStyle(NShapeDraw::Rect)->SetFillColor(MakeArgb(255, 255, 255, 255))->SetBorderWidth(0);
            SetBkgDraw(bkgDraw);
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

        HWND NNative::GetNative() const
        {
            return realWindow_;
        }

        void NNative::Attach(NFrame* parentFrame, HWND hwndNative)
        {
            NAssertError(hwndNative && ::IsWindow(hwndNative), _T("Invalid window handle"));

            Create(parentFrame);
            AttachWnd(hwndNative);
        }

        NNative* NNative::GetNativeUi(HWND hWnd)
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

        void NNative::OnFocus()
        {
            __super::OnFocus();

            if(!window_)
                return;

            if(realWindow_ == NULL)
            {
                CreateRealWindow();
                OnRealWindowCreated();
            }
            NAssertError(::IsWindow(realWindow_), _T("failed to create window"));
            ::SetFocus(realWindow_);
        }

        void NNative::OnSizeChanged(int width, int height)
        {
            __super::OnSizeChanged(width, height);

            if(realWindow_)
            {
                Base::NRect rcWnd = GetRootRect();
                ::SetWindowPos(realWindow_, NULL, rcWnd.Left, rcWnd.Top, rcWnd.Width(), rcWnd.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
            }

        }

        void NNative::OnPosChanged(int left, int top)
        {
            __super::OnPosChanged(left, top);

            if(realWindow_)
            {
                Base::NRect rcWnd = GetRootRect();
                ::SetWindowPos(realWindow_, NULL, rcWnd.Left, rcWnd.Top, rcWnd.Width(), rcWnd.Height(), SWP_NOZORDER | SWP_NOACTIVATE);
            }
        }

        void NNative::OnLostFocus()
        {
            __super::OnLostFocus();

            if(IsWndValid())
            {
                TCHAR fixedBuffer[1024];
                TCHAR* buffer = fixedBuffer;
                int textLength = ::GetWindowTextLength(realWindow_) + 1;
                if(textLength > _countof(fixedBuffer))
                    buffer = NNewArray(TCHAR, textLength);
                textLength = ::GetWindowText(realWindow_, buffer, textLength);
                buffer[textLength] = 0;
                ::SetWindowText(realWindow_, buffer);
                if(buffer != fixedBuffer)
                    NDeleteArray(buffer);
                ::DestroyWindow(realWindow_);
                realWindow_ = NULL;
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

        void NNative::OnRealWindowCreated()
        {
            ;
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

            bool visible = IsVisible();
            bool enabled = IsEnabled();


            nui::Base::NPoint point(rcWnd.left, rcWnd.top);
            NFrameBase* parentFrame = parentFrame_;
            while(parentFrame != NULL)
            {
                const nui::Base::NRect& rect = parentFrame->GetRect();
                point.X -= rect.Left;
                point.Y -= rect.Top;
                visible = visible && parentFrame->IsVisible();
                enabled = enabled && parentFrame->IsEnabled();
                parentFrame = parentFrame->GetParent();
            }
            SetPos(point.X, point.Y);
            SetSize(rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);
            ::ShowWindow(realWindow_, visible ? TRUE : FALSE);
            ::EnableWindow(realWindow_, enabled ? TRUE : FALSE);
        }

        void NNative::CreateRealWindow()
        {
            Base::NString wndClassName;
            DWORD style;
            DWORD exStyle;
            if(!GetWndData(wndClassName, style, exStyle))
                return;

            bool visible = IsVisible();
            bool enabled = IsEnabled();

            NFrameBase* parentFrame = parentFrame_;
            while(parentFrame != NULL)
            {
                visible = visible && parentFrame->IsVisible();
                enabled = enabled && parentFrame->IsEnabled();
                parentFrame = parentFrame->GetParent();
            }

            Base::NRect rcWnd = GetRootRect();
            HWND hWnd = ::CreateWindowEx(exStyle,
                wndClassName,
                GetText().GetData(),
                WS_TABSTOP | WS_CHILD | (visible ? WS_VISIBLE : 0) | style,
                rcWnd.Left, rcWnd.Top,
                rcWnd.Width(), rcWnd.Height(),
                window_->GetNative(),
                NULL,
                ::GetModuleHandle(NULL),
                0);
            if(!enabled)
                ::EnableWindow(hWnd, FALSE);
            AttachWnd(hWnd);

            HFONT hFont = GdiObjMgr::Instance().GetDefaultFont();
            ::SendMessage(hWnd, WM_SETFONT, reinterpret_cast<WPARAM>(hFont), TRUE);
        }
    }
}
