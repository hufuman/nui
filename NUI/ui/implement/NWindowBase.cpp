#include "stdafx.h"
#include "../NWindowBase.h"


#include "WindowDef.h"
#include "WindowUtil.h"
#include "WindowMap.h"

#include "../NMsgLoop.h"

namespace NUI
{
    namespace UI
    {
        NWindowBase::NWindowBase()
        {
            window_ = NULL;
        }

        NWindowBase::~NWindowBase()
        {
            NAssertError(window_ == NULL, _T("window_ isn't Null in ~WindowBase"));
        }

        void NWindowBase::SetMsgFilterCallback(MsgFilterCallback callback)
        {
            msgFilterCallback_ = callback;
        }

        bool NWindowBase::Create(HWND parentWindow, LPCTSTR szText, NUI::Base::NRect& rect)
        {
            NAssertError(window_ == NULL, _T("Window already exists"));

            bool result = Util::EnsureWindowClass(SKIN_WINDOW_NAME, &NWindowBase::_staticWndProc);
            NAssertError(result, _T("Failed to register class"));
            if(!result)
                return false;

            window_ = ::CreateWindowEx(SKIN_WINDOW_EXSTYLE, SKIN_WINDOW_NAME, szText,
                SKIN_WINDOW_STYLE,
                rect.Left, rect.Top,
                rect.Width(), rect.Height(),
                parentWindow,
                NULL,
                NUI::Data::NModule::GetInst().GetNUIModule(),
                static_cast<LPVOID>(this));
            NAssertError(window_ != NULL, _T("Failed to create window"));

            return (window_ != NULL);
        }

        bool NWindowBase::DoModal(HWND parentWindow, LPCTSTR szText, NUI::Base::NRect& rect)
        {
            NAssertError(window_ == NULL, _T("window_ isn't Null in DoModal"));
            if(parentWindow != NULL)
            {
                ::EnableWindow(parentWindow, FALSE);
            }

            bool result = Create(parentWindow, szText, rect);
            if(result)
            {
                NUI::UI::NMsgLoop loop;
                result = loop.Loop(window_);
            }

            if(parentWindow != NULL)
            {
                ::EnableWindow(parentWindow, TRUE);
            }
            return result;
        }

        void NWindowBase::Destroy()
        {
            NAssertError(window_ != NULL && ::IsWindow(window_), _T("Invalid window in WindowBase::Destroy"));
            if(window_ != NULL)
            {
                if(::IsWindow(window_))
                    ::DestroyWindow(window_);
                window_ = NULL;
            }
        }

        void NWindowBase::ShowWindow(DWORD showFlag)
        {
            NAssertError(window_ != NULL && ::IsWindow(window_), _T("Invalid window in WindowBase::ShowWindow"));
            if(window_ != NULL)
                ::ShowWindow(window_, showFlag);
        }

        HWND NWindowBase::GetNative() const
        {
            return window_;
        }

        LRESULT NWindowBase::_staticWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
        {
            NWindowBase* pThis = NUI::UI::Util::WindowMap::GetInst().GetWindow(window);
            if(message == WM_NCCREATE)
            {
                CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                pThis = static_cast<NWindowBase*>(cs->lpCreateParams);
                pThis->window_ = window;
                NUI::UI::Util::WindowMap::GetInst().AddWindow(window, pThis);
            }

            LRESULT lResult = 0;
            if(pThis == NULL || !pThis->OnMessage(message, wParam, lParam, lResult))
                lResult = ::CallWindowProc(::DefWindowProc, window, message, wParam, lParam);
            if(message == WM_NCDESTROY)
            {
                if(pThis != NULL)
                    pThis->window_ = NULL;
                NUI::UI::Util::WindowMap::GetInst().RemoveWindow(window);
            }
            return lResult;
        }

        bool NWindowBase::OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
        {
            if(msgFilterCallback_ && msgFilterCallback_(window_, message, wParam, lParam, lResult))
                return true;

            lResult = 0;
            return false;
        }

    }
}
