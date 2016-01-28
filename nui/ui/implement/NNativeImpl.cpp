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

            NAssertError(realWindow_ == NULL, _T("real window created already"));
            CreateRealWindow();
            OnRealWindowCreated();
            ::SetFocus(realWindow_);
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

            SetPos(rcWnd.left, rcWnd.top);
            SetSize(rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

            bool visible = IsVisible();
            bool enabled = IsEnabled();

            NFrameBase* parentFrame = parentFrame_;
            while(parentFrame != NULL)
            {
                visible = visible && parentFrame->IsVisible();
                enabled = enabled && parentFrame->IsEnabled();
                parentFrame = parentFrame->GetParent();
            }
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
