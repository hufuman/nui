#include "stdafx.h"
#include "../NWindowBase.h"

#include <Mmsystem.h>
#pragma comment(lib, "Winmm.lib")

#include "WindowDef.h"
#include "WindowUtil.h"
#include "WindowMap.h"

#include "../NMsgLoop.h"

#define WM_EVENT_TIMER WM_USER+100

namespace nui
{
    namespace Ui
    {
        NWindowBase::NWindowBase() : msgLoop_(MemToolParam)
        {
            windowStyle_ = WindowStyle::Top | WindowStyle::Sizable;
            window_ = NULL;
            mouseTracking_ = false;
            invalidateRgn_ = NULL;
            lastDrawTick_ = 0;
            drawTimerId_ = 0;
            modalParent_ = NULL;
            privateWindowBaseData_ = NULL;
			minRestore_ = false;
        }

        NWindowBase::~NWindowBase()
        {
            NAssertError(window_ == NULL, _T("window_ isn't Null in ~WindowBase"));
        }

        void NWindowBase::SetMsgFilterCallback(MsgFilterCallback callback)
        {
            msgFilterCallback_ = callback;
        }

        bool NWindowBase::Create(HWND parentWindow)
        {
            return Create(parentWindow, windowStyle_);
        }

        bool NWindowBase::Create(HWND parentWindow, DWORD styleValue)
        {
            NAssertError(window_ == NULL, _T("Window already exists"));

            bool result = Util::EnsureWindowClass(SKIN_WINDOW_NAME, &NWindowBase::_staticWndProc);
            NAssertError(result, _T("Failed to register class"));
            if(!result)
                return false;

            windowStyle_ = styleValue;
            DWORD style, exStyle;
            GetStyle(style, exStyle);

            window_ = ::CreateWindowEx(exStyle,
                SKIN_WINDOW_NAME,
                _T(""),
                style,
                0, 0,
                CW_USEDEFAULT, CW_USEDEFAULT,
                parentWindow,
                NULL,
                nui::Data::NModule::GetInst().GetNUIModule(),
                static_cast<LPVOID>(this));
            NAssertError(window_ != NULL, _T("Failed to create window"));

            if(privateWindowBaseData_)
            {
                SetText(privateWindowBaseData_->text);
                SetRect(privateWindowBaseData_->rect);
                if(privateWindowBaseData_->centerWindow)
                    CenterWindow(privateWindowBaseData_->centerRelativeWindow);
                SetVisible(privateWindowBaseData_->visible);
                NSafeRelease(privateWindowBaseData_);
            }

            if(window_)
            {
                OnCreate();
            }

            return (window_ != NULL);
        }

        bool NWindowBase::DoModal(HWND parentWindow)
        {
            NAssertError(window_ == NULL, _T("window_ isn't Null in DoModal"));
            modalParent_ = parentWindow;

            //if(privateWindowBaseData_)
            //    privateWindowBaseData_->AddRef();

            bool result = Create(modalParent_);
            if(result)
            {
                CenterWindow(modalParent_);
                SetVisible(true);

                if(modalParent_ != NULL)
                    ::EnableWindow(modalParent_, FALSE);

                result = msgLoop_->Loop(window_);
            }

			return result;
        }

        void NWindowBase::Destroy()
        {
            NAssertError(window_ != NULL && ::IsWindow(window_), _T("Invalid window in WindowBase::Destroy"));
            if(window_ != NULL)
            {
				if (::IsWindow(window_))
				{
					::SendMessage(window_, WM_CLOSE, 0, 0);
				}
                window_ = NULL;
                ::PostThreadMessage(::GetCurrentThreadId(), WM_NULL, 0, 0);
            }
        }

        void NWindowBase::SetStyle(DWORD styleValue)
        {
            NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::SetStyle"));
            windowStyle_ = styleValue;
            if(window_ != NULL)
            {
                DWORD dwStyle;
                DWORD dwExStyle;
                GetStyle(dwStyle, dwExStyle);
                ::SetWindowLongPtr(window_, GWL_STYLE, dwStyle);
                ::SetWindowLongPtr(window_, GWL_EXSTYLE, dwExStyle);
            }
        }

        void NWindowBase::SetVisible(bool visible)
        {
            NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::SetVisible"));
            if(window_ == NULL)
            {
                GetPrivateData()->visible = visible;
            }
            else
            {
                if(visible)
                {
                    bool noActivate = ((windowStyle_ & WindowStyle::MenuLike) == WindowStyle::MenuLike);
                    ::ShowWindow(window_, noActivate ? SW_SHOWNOACTIVATE : SW_SHOW);
                    if(::IsIconic(window_) || ::GetForegroundWindow() != window_)
                        ::SwitchToThisWindow(window_, TRUE);
                }
                else
                {
                    ::ShowWindow(window_, SW_HIDE);
                }
            }
        }

		bool NWindowBase::IsVisible()
		{
			NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::SetVisible"));
			if (window_ == NULL)
			{
				return GetPrivateData()->visible;
			}
			else
			{
				return ::IsWindowVisible(window_) ? true : false;
			}
		}

        bool NWindowBase::GetRect(nui::Base::NRect& rect)
        {
            NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::GetRect"));

            if(window_ != NULL)
                return !!::GetWindowRect(window_, rect);

            if(privateWindowBaseData_)
            {
                rect = privateWindowBaseData_->rect;
                return true;
            }
            return false;
        }

        void NWindowBase::SetPos(int x, int y)
        {
            NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::SetPos"));

            if(window_ == NULL)
                GetPrivateData()->rect.SetPos(x, y);
            else
                ::SetWindowPos(window_, NULL, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
        }

        void NWindowBase::SetSize(int width, int height)
        {
            NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::SetSize"));

            if(::IsZoomed(window_) || ::IsIconic(window_))
                return;

            if(window_ == NULL)
                GetPrivateData()->rect.SetSize(width, height);
            else
                ::SetWindowPos(window_, NULL, 0, 0, width, height, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOMOVE);
        }

        void NWindowBase::CenterWindow(HWND relativeWindow)
        {
            NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::CenterWindow"));
            if(window_ == NULL)
            {
                GetPrivateData()->centerWindow = true;
                GetPrivateData()->centerRelativeWindow = relativeWindow;
                return;
            }

            BOOL bDeskWnd = FALSE;
            DWORD dwStyle = ::GetWindowLongPtr(window_, GWL_STYLE);
            // Find correct window
            if(relativeWindow == NULL)
            {
                if (dwStyle & WS_CHILD)
                    relativeWindow = ::GetParent(window_);
                else
                    relativeWindow = ::GetWindow(window_, GW_OWNER);
                if(relativeWindow == NULL)
                {
                    bDeskWnd = TRUE;
                    relativeWindow = ::GetDesktopWindow();
                }
            }

            // Calc correct position
            RECT rcWnd = {0};
            RECT rcWndRelative = {0};
            ::GetWindowRect(window_, &rcWnd);

            if(IsIconic(relativeWindow))
            {
                // 最小化的时候，取Restored的大小
                WINDOWPLACEMENT wndPlacement = {0};
                wndPlacement.length = sizeof(wndPlacement);
                ::GetWindowPlacement(relativeWindow, &wndPlacement);
                memcpy(&rcWndRelative, &wndPlacement.rcNormalPosition, sizeof(rcWndRelative));
            }
            else
            {
                if(bDeskWnd)
                    ::SystemParametersInfo(SPI_GETWORKAREA, 0, (PVOID)&rcWndRelative, FALSE);
                else
                    ::GetWindowRect(relativeWindow, &rcWndRelative);
            }
            int nTop = (rcWndRelative.bottom - rcWndRelative.top) - (rcWnd.bottom - rcWnd.top);
            int nLeft = (rcWndRelative.right - rcWndRelative.left) - (rcWnd.right - rcWnd.left);

            nLeft /= 2;
            nLeft += rcWndRelative.left;
            nTop /= 2;
            nTop += rcWndRelative.top;

            ::SetWindowPos(window_, NULL, nLeft, nTop, 0, 0, SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
        }

        void NWindowBase::SetRect(const Base::NRect& rect)
        {
            NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::SetRect"));
            if(window_ == NULL)
                GetPrivateData()->rect = rect;
            else
                ::SetWindowPos(window_, NULL, rect.Left, rect.Top, rect.Width(), rect.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
        }

		void NWindowBase::SetIcon(HICON icon, UINT type)
		{
			if(type == ICON_BIG)
				GetPrivateData()->bigIcon_ = icon;
			else 
				GetPrivateData()->smallIcon_ = icon;
			SendMessage(window_, WM_SETICON, type, (LPARAM)icon);
		}

        void NWindowBase::Invalidate()
        {
            Base::NRect rcClient;
            ::GetClientRect(window_, rcClient);
            InvalidateRect(rcClient);
        }

		void WINAPI onTimeFunc(UINT wTimerID, UINT /*msg*/, DWORD_PTR dwUser, DWORD_PTR /*dwl*/, DWORD_PTR /*dw2*/)
		{
			::PostMessage((HWND)dwUser, WM_EVENT_TIMER, wTimerID, 0);
			return;
		}

        void NWindowBase::InvalidateRect(const Base::NRect& rect)
        {
            if(window_ == NULL)
                return;
			if (drawTimerId_ == 0)
				drawTimerId_ = ::timeSetEvent(30, 1, (LPTIMECALLBACK)onTimeFunc, (DWORD_PTR)window_, TIME_PERIODIC);
                //drawTimerId_ = ::SetTimer(window_, 1000, 30, NULL);

            // when invalidateRgn_ is null, the whole window need to be redrawn
            HRGN tempRgn = ::CreateRectRgn(rect.Left, rect.Top, rect.Right, rect.Bottom);
            ::CombineRgn(invalidateRgn_, invalidateRgn_, tempRgn, RGN_OR);
            ::DeleteObject(tempRgn);
        }

        void NWindowBase::SetText(LPCTSTR text)
        {
            NAssertError(window_ == NULL || ::IsWindow(window_), _T("Invalid window in WindowBase::SetText"));
            if(window_ == NULL)
                GetPrivateData()->text = text;
            else
                ::SetWindowText(window_, text);
        }

        HWND NWindowBase::GetNative() const
        {
            return window_;
        }

        LRESULT NWindowBase::DoDefault(UINT message, WPARAM wParam, LPARAM lParam)
        {
            if(window_ == NULL)
                return 0;
            NAssertError(!!::IsWindow(window_), _T("Invalid window in WindowBase::DoDefault"));
            if(window_ != NULL)
                return ::CallWindowProc(::DefWindowProc, window_, message, wParam, lParam);
            return 0;
        }

        LRESULT NWindowBase::_staticWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
        {
            NWindowBase* pThis = nui::Ui::Util::WindowMap::GetInst().GetWindow(window);
            if(message == WM_NCCREATE)
            {
                CREATESTRUCT *cs = reinterpret_cast<CREATESTRUCT*>(lParam);
                pThis = static_cast<NWindowBase*>(cs->lpCreateParams);
                pThis->window_ = window;
                nui::Ui::Util::WindowMap::GetInst().AddWindow(window, pThis);
            }

            LRESULT lResult = 0;
            if(pThis == NULL)
                lResult = ::CallWindowProc(::DefWindowProc, window, message, wParam, lParam);
            else if(!pThis->OnMessage(message, wParam, lParam, lResult))
                lResult = pThis->DoDefault(message, wParam, lParam);

            if(message == WM_NCDESTROY && pThis != NULL)
            {
                pThis->window_ = NULL;
                nui::Ui::Util::WindowMap::GetInst().RemoveWindow(window);
            }
            return lResult;
        }

        bool NWindowBase::OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
        {
            if(!mouseTracking_ && message == WM_MOUSEMOVE)
            {
                TRACKMOUSEEVENT _Event = {0};
                _Event.cbSize = sizeof(_Event);
                _Event.dwFlags = TME_LEAVE;
                _Event.hwndTrack = window_;
                if(::TrackMouseEvent(&_Event))
                {
                    mouseTracking_ = true;
                }
            }
            else if(message == WM_SHOWWINDOW)
            {
                if(wParam)
                {
                    Invalidate();
                    ::SetFocus(window_);
                }
			}
			else if (message == WM_SYSCOMMAND)
			{
				if (wParam == SC_RESTORE)
				{
					minRestore_ = true;
					Invalidate();
				}
			}
            else if(message == WM_KILLFOCUS && ((windowStyle_ & WindowStyle::MenuLike) == WindowStyle::MenuLike))
            {
                SetVisible(false);
            }
            else if(message == WM_MOUSELEAVE)
            {
                mouseTracking_ = false;
            }
            else if(message == WM_EVENT_TIMER)
            {
                if(wParam == drawTimerId_ && !IsRegionEmpty(invalidateRgn_))
                {
                    if(IsLayered())
                    {
                        ::PostMessage(window_, WM_PAINT, 0, 0);
                    }
                    else
                    {
                        ::InvalidateRgn(window_, invalidateRgn_, FALSE);
						if (minRestore_)
						{
							minRestore_ = false;
							::RedrawWindow(window_, NULL, invalidateRgn_, RDW_UPDATENOW);
						}
                    }
                    ResetInvalidRgn();
                }
            }
            else if(message == WM_PAINT)
            {
                PAINTSTRUCT ps = {0};
                HDC hDc = ::BeginPaint(window_, &ps);

                if(!IsLayered() && !IsRegionEmpty(invalidateRgn_))
                {
                    ::ExtSelectClipRgn(hDc, invalidateRgn_, RGN_OR);
                }

                Draw(hDc);
                ::EndPaint(window_, &ps);
            }
            else if(message == WM_PRINT)
            {
                HDC hDc = (HDC)wParam;
                Draw(hDc);
            }
            else if(message == WM_DESTROY)
            {
                if(drawTimerId_ != 0)
                {
					::timeKillEvent(drawTimerId_);
                    //::KillTimer(window_, drawTimerId_);
                    drawTimerId_ = 0;
                }
            }
            else if(message == WM_CLOSE)
            {
				OnClose();

                if(modalParent_ != NULL && !::IsWindowEnabled(modalParent_))
                    ::EnableWindow(modalParent_, TRUE);

                modalParent_ = NULL;
            }
            else if(message == WM_MOUSEACTIVATE && ((windowStyle_ & WindowStyle::MenuLike) == WindowStyle::MenuLike))
            {
                lResult = MA_NOACTIVATE;
                return true;
            }

            if(msgFilterCallback_ && msgFilterCallback_(this, message, wParam, lParam, lResult))
                return true;

            lResult = 0;
            return false;
        }

        void NWindowBase::OnCreate()
        {
            ResetInvalidRgn();
            Invalidate();
        }

        void NWindowBase::Draw(HDC hDc)
        {
            UNREFERENCED_PARAMETER(hDc);
        }

		void NWindowBase::OnClose()
		{

		}

        bool NWindowBase::IsRegionEmpty(HRGN clipRgn)
        {
            static HRGN emptyRegion = ::CreateRectRgn(0, 0, 0, 0);
            return !!::EqualRgn(emptyRegion, clipRgn);
        }

        void NWindowBase::ResetInvalidRgn()
        {
            if(invalidateRgn_ != NULL)
                ::DeleteObject(invalidateRgn_);

            invalidateRgn_ = ::CreateRectRgn(0, 0, 0, 0);
        }

        bool NWindowBase::IsLayered() const
        {
            return ((windowStyle_ & WindowStyle::Layered) == WindowStyle::Layered);
        }

        void NWindowBase::GetStyle(DWORD& style, DWORD& exStyle) const
        {
            style = 0;
            exStyle = 0;
            if(windowStyle_ & WindowStyle::Child)
            {
                style |= WS_CHILD;
            }
            if(windowStyle_ & WindowStyle::Layered)
            {
                exStyle |= WS_EX_LAYERED;
            }
            if(windowStyle_ & WindowStyle::Sizable)
            {
                style |= WS_SIZEBOX | WS_OVERLAPPED;
            }
            if(windowStyle_ & WindowStyle::Transparent)
            {
                exStyle |= WS_EX_TRANSPARENT;
            }
            if((windowStyle_ & WindowStyle::MenuLike) == WindowStyle::MenuLike)
            {
                exStyle |= WS_EX_TOOLWINDOW | WS_EX_TOPMOST;
            }
            if((windowStyle_ & WindowStyle::Top) || windowStyle_ == 0)
            {
                style |= WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED | WS_THICKFRAME;
            }
            style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
        }

        nui::Base::NAutoPtr<NWindowBase::WindowBasePrivateData> NWindowBase::GetPrivateData()
        {
            if(privateWindowBaseData_)
                return privateWindowBaseData_;
            privateWindowBaseData_ = NNew(WindowBasePrivateData);
            privateWindowBaseData_->AddRef();
            return privateWindowBaseData_;
        }
    }
}
