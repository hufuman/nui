#include "stdafx.h"

#include "../NFrame.h"
#include "../NWindow.h"
#include "./Gdi/GdiCursor.h"


namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NWindow);

        NWindow::NWindow()
        {
            tooltipWnd_ = NULL;
            sizableBorder_.SetRect(4, 4, 4, 4);
            render_ = NUiBus::Instance().CreateRender();
        }

        NWindow::~NWindow()
        {
        }

#ifndef _NO_NUI_PARSER_
        bool NWindow::DoModalWithStyle(HWND parentWindow, LPCTSTR styleName)
        {
            if(styleName && styleName[0] != 0)
                styleName_ = styleName;
            else
                styleName_ = _T("");
            return __super::DoModal(parentWindow);
        }
#endif  // _NO_NUI_PARSER_

        NFrame* NWindow::GetRootFrame()
        {
            if(rootFrame_ == NULL)
            {
                Base::NInstPtr<NFrame> rootFrame(MemToolParam);
                rootFrame_ = (NFrame*)rootFrame;
                rootFrame_->SetId(_NUI_ROOT_FRAME_ID_);
                rootFrame_->window_ = this;
                rootFrame_->SetAutoSize(false);
                // rootFrame->SetLayout(NFrameBase::LayoutHFill | NFrameBase::LayoutVFill);
                Base::NRect rect;
                GetRect(rect);
                rootFrame_->SetSize(rect.Width(), rect.Height());
                rootFrame_->SizeEvent.AddHandler(MakeDelegate(this, &NWindow::OnRootFrameSizeChanged));
                rootFrame_->TextChangedEvent.AddHandler(MakeDelegate(this, &NWindow::OnRootFrameTextChanged));
            }
            return rootFrame_;
        }

        NRender* NWindow::GetRender() const
        {
            return render_;
        }

        // WindowMsgFilter
        bool NWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
        {
            if(NWindowBase::OnMessage(message, wParam, lParam, lResult))
                return true;

            switch(message)
            {
            case WM_COMMAND:
                if(OnWndCmd(wParam, lParam))
                {
                    lResult = 0;
                    return true;
                }
                break;
            case WM_DESTROY:
                render_ = NULL;
                if(rootFrame_ != NULL)
                    rootFrame_->window_ = NULL;
                rootFrame_ = NULL;
                DestroyTooltipWnd();
                break;
            case WM_ERASEBKGND:
                lResult = 1;
                return true;
            case WM_NCACTIVATE:
                {
                    if(::IsIconic(window_))
                        return false;
                    lResult = (wParam == 0) ? TRUE : FALSE;
                    return true;
                }
            case WM_SETCURSOR:
                {
                    if(hoverFrame_ != NULL)
                    {
                        NCursor* cursor = hoverFrame_->GetCursor();
                        if(cursor != NULL)
                        {
                            Gdi::GdiCursor* gdiCursor = dynamic_cast<Gdi::GdiCursor*>(cursor);
                            ::SetCursor(gdiCursor->GetCursor());
                            lResult = TRUE;
                            return true;
                        }
                    }
                    break;
                }
            case WM_NCHITTEST:
                {
                    lResult = HTCLIENT;
                    if((::GetWindowLongPtr(window_, GWL_STYLE) & WS_SIZEBOX) == WS_SIZEBOX)
                    {
                        Base::NRect rcWnd;
                        GetRect(rcWnd);
                        POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                        if(pt.x <= rcWnd.Left + sizableBorder_.Left)
                        {
                            if(pt.y <= rcWnd.Top + sizableBorder_.Top)
                                lResult = HTTOPLEFT;
                            else if(pt.y >= rcWnd.Bottom - sizableBorder_.Bottom)
                                lResult = HTBOTTOMLEFT;
                            else
                                lResult = HTLEFT;
                        }
                        else if(pt.x >= rcWnd.Right - sizableBorder_.Right)
                        {
                            if(pt.y <= rcWnd.Top + sizableBorder_.Top)
                                lResult = HTTOPRIGHT;
                            else if(pt.y >= rcWnd.Bottom - sizableBorder_.Bottom)
                                lResult = HTBOTTOMRIGHT;
                            else
                                lResult = HTRIGHT;
                        }
                        else if(pt.y <= rcWnd.Top + sizableBorder_.Top)
                        {
                            lResult = HTTOP;
                        }
                        else if(pt.y >= rcWnd.Bottom - sizableBorder_.Bottom)
                        {
                            lResult = HTBOTTOM;
                        }
                    }
                    if(lResult == HTCLIENT)
                    {
                        Base::NPoint point(LOWORD(lParam), HIWORD(lParam));
                        ::ScreenToClient(window_, reinterpret_cast<LPPOINT>(&point));
                        NFrame* newFrame = RefreshHoverItem(point);
                        lResult = newFrame == NULL ? HTCAPTION : HTCLIENT;
                    }
                    return true;
                }
            case WM_NCPAINT:
                {
                    lResult = 0;
                    return true;
                }
            case WM_NCCALCSIZE:
                {
                    NCCALCSIZE_PARAMS* params = reinterpret_cast<NCCALCSIZE_PARAMS*>(lParam);
                    if(::IsZoomed(window_))
                    {
                        params->rgrc[0].right += params->rgrc[0].left;
                        params->rgrc[0].bottom += params->rgrc[0].top;
                        params->rgrc[0].left = 0;
                        params->rgrc[0].top = 0;
                        lResult = 0;
                        return true;
                    }
                    lResult = 0;
                    return true;
                }
            case WM_SIZE:
                if(wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
                {
                    lResult = DoDefault(message, wParam, lParam);
                    OnSize(LOWORD(lParam), HIWORD(lParam));
                    return true;
                }
                break;
            case WM_ACTIVATE:
                {
                    BOOL bActive = (LOWORD(wParam) != WA_INACTIVE);
                    if(!bActive)
                    {
                        SetHoverItem(NULL);
                        NUiBus::Instance().SetCaptureFrame(NULL);
                    }
                    break;
                }
            case WM_MOUSELEAVE:
                SetHoverItem(NULL);
                break;
            case WM_MOUSEMOVE:
                {
                    NFrame* frame = NULL;
                    Base::NPoint point(LOWORD(lParam), HIWORD(lParam));
                    NFrameBase* capturedFrame = NUiBus::Instance().GetCaptureFrame();
                    if(capturedFrame != NULL)
                    {
                        frame = dynamic_cast<NFrame*>(capturedFrame);
                    }
                    else
                    {
                        NFrame* newFrame = RefreshHoverItem(point);
                        SetHoverItem(newFrame);
                        frame = hoverFrame_;
                    }
                    if(frame)
                        frame->OnMouseMove(static_cast<short>(point.X), static_cast<short>(point.Y));
                }
                break;
            case WM_LBUTTONDBLCLK:
            case WM_LBUTTONDOWN:
                {
                    Base::NPoint point(LOWORD(lParam), HIWORD(lParam));
                    NFrame* newFrame = RefreshHoverItem(point);
                    SetHoverItem(newFrame);

                    if(hoverFrame_)
                    {
                        hoverFrame_->OnMouseDown(static_cast<short>(point.X), static_cast<short>(point.Y));

                        if(hoverFrame_ != focusFrame_ && focusFrame_ != NULL)
                            focusFrame_->OnLostFocus();

                        focusFrame_ = hoverFrame_;
                        focusFrame_->OnFocus();
                    }

                    if(hoverFrame_ == NULL)
                    {
                        ::SendMessage(window_, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
                    }
                    else
                    {
                        ::SetCapture(window_);
                        NUiBus::Instance().SetCaptureFrame(hoverFrame_);
                    }
                }
                break;
            case WM_CAPTURECHANGED:
                NUiBus::Instance().SetCaptureFrame(NULL);
                break;
            case WM_LBUTTONUP:
                if(::GetCapture() == window_)
                {
                    if(hoverFrame_ != NULL)
                    {
                        Base::NPoint point(LOWORD(lParam), HIWORD(lParam));
                        Base::NRect rcFrame = hoverFrame_->GetRootRect();
                        if(rcFrame.Contains(point))
                            hoverFrame_->OnClicked(point);
                    }
                    NUiBus::Instance().SetCaptureFrame(NULL);
                    ::ReleaseCapture();
                }
                break;
            case WM_KEYUP:
                lResult = 0;
                HandleKeyEvent(static_cast<TCHAR>(wParam), false);
                return true;
            case WM_KEYDOWN:
                lResult = 0;
                HandleKeyEvent(static_cast<TCHAR>(wParam), true);
                return true;
            }
            return false;
        }

        bool NWindow::HandleKeyEvent(TCHAR key, bool isDownEvent)
        {
            NFrame* frame = focusFrame_;
            if(frame == NULL)
                frame = rootFrame_;

            if(frame == NULL)
                return true;

            bool result = false;
            while(!result && frame != NULL)
            {
                if(isDownEvent)
                    result = frame->OnKeyDown(key);
                else
                    result = frame->OnKeyUp(key);
                frame = frame->GetParent();
            }

            return true;
        }

        bool NWindow::OnWndCmd(WPARAM wParam, LPARAM lParam)
        {
            if(wParam == NULL || !::IsWindow(reinterpret_cast<HWND>(lParam)))
                return false;
            NNative* wndUi = NNative::GetNativeUi(reinterpret_cast<HWND>(lParam));
            if(wndUi == NULL)
                return false;
            return wndUi->OnParentCommand(HIWORD(wParam));
        }

        void NWindow::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            {
                NAssertTempDisable();
                GetRootFrame()->ApplyStyle(_T("@sys_default_style:window"));
            }
#endif  // _NO_NUI_PARSER_

            if(!styleName_.IsEmpty())
            {
                GetRootFrame()->ApplyStyle(styleName_);
            }

            SyncSysButtonGroup();

            WindowCreatedEvent.Invoke(this, NULL);
        }

        void NWindow::OnSize(int width, int height)
        {
            UpdateTooltipWnd();
            if(rootFrame_)
            {
                rootFrame_->SetSize(width, height);

                if(btnSysMax_)
                {
                    btnSysMax_->SetCheck(!!::IsZoomed(window_));
                    btnSysMax_->SetTooltip(btnSysMax_->IsChecked() ? _T("@Common:btnSysRestore") :  _T("@Common:btnSysMax"));
                }
            }

            Base::NRect rcWnd;
            GetRect(rcWnd);
            HRGN rgn = ::CreateRectRgn(0, 0, rcWnd.Width(), rcWnd.Height());
            if(rgn != NULL)
                ::SetWindowRgn(GetNative(), rgn, FALSE);
        }

        void NWindow::OnDraw(NRender* render, HRGN clipRgn)
        {
            Base::NRect rcClient;
            GetRect(rcClient);
            rcClient.Offset(-rcClient.Left, -rcClient.Top);

            WindowDrawEventData eventData;
            eventData.render = render;
            eventData.region = clipRgn;
            PreDrawEvent.Invoke(this, &eventData);

            if(rootFrame_ != NULL)
            {
                Base::NPoint pt;
                rootFrame_->Draw(render, pt, clipRgn);
            }

            PostDrawEvent.Invoke(this, &eventData);
        }

        void NWindow::Draw(HDC hDc)
        {
            GUARD_SCOPE(false, _T("NWindow Draw takes too long"));
            HRGN clipRgn = ::CreateRectRgn(0, 0, 0, 0);
            int clipResult = ::GetClipRgn(hDc, clipRgn);

            Base::NRect clientRect;
            ::GetClientRect(window_, clientRect);

            if(clipResult != 1)
            {
                ::DeleteObject(clipRgn);
                clipRgn = ::CreateRectRgn(clientRect.Left, clientRect.Top, clientRect.Right, clientRect.Bottom);
            }
            render_->Init(hDc, clientRect);

            OnDraw(render_, clipRgn);

            render_->DrawBack(IsLayered());
            ::DeleteObject(clipRgn);
        }

        void NWindow::SetHoverItem(NFrame* frame)
        {
            if(hoverFrame_ == frame)
                return;

            HideTooltip();
            if(hoverFrame_ && hoverFrame_ != frame)
            {
                hoverFrame_->OnMouseLeave();
            }

            NFrameBase* capturedFrame = NUiBus::Instance().GetCaptureFrame();
            if(frame != NULL && capturedFrame != frame && capturedFrame != NULL)
                return;

            hoverFrame_ = frame;
            if(hoverFrame_)
            {
                hoverFrame_->OnMouseHover();
                Base::NString tooltip = hoverFrame_->GetTooltip();
                ShowTooltip(tooltip);
            }
        }

        NFrame* NWindow::RefreshHoverItem(const Base::NPoint& point)
        {
            if(rootFrame_ == NULL)
                return NULL;
            NFrame* newHover = dynamic_cast<NFrame*>(rootFrame_->GetChildByPointAndFlag(point, NFrameBase::FlagCanHover, false));
            return newHover;
        }

        bool NWindow::OnRootFrameSizeChanged(Base::NBaseObj*, NEventData* eventData)
        {
            NFrameBase::SizeEventData* data = static_cast<NFrameBase::SizeEventData*>(eventData);
            SetSize(data->width, data->height);
            return true;
        }

        bool NWindow::OnRootFrameTextChanged(Base::NBaseObj*, NEventData*)
        {
            if(window_)
                ::SetWindowText(window_, rootFrame_->GetText());
            return true;
        }

        void NWindow::SyncSysButtonGroup()
        {
            if(rootFrame_ == NULL)
                return;

            NFrame* sysButtonGroup = dynamic_cast<NFrame*>(rootFrame_->GetChildById(_NUI_SYS_BUTTON_GROUP_ID_, false));
            if(!sysButtonGroup)
                return;

            btnSysMin_ = sysButtonGroup->GetChildById<NFrame*>(_NUI_SYS_MIN_BUTTON_ID_);
            if(btnSysMin_ != NULL)
            {
                btnSysMin_->ClickEvent.AddHandler(MakeDelegate(this, &NWindow::OnBtnMinClickedChanged));
            }

            NFrame* frameSysMax = sysButtonGroup->GetChildById<NFrame*>(_NUI_SYS_MAX_BUTTON_ID_);
            if(frameSysMax != NULL)
            {
                btnSysMax_ = dynamic_cast<NCheckBox*>(frameSysMax);
                btnSysMax_->SetCheck(!!::IsZoomed(window_));
                btnSysMax_->SetTooltip(btnSysMax_->IsChecked() ? _T("@Common:btnSysRestore") :  _T("@Common:btnSysMax"));
                btnSysMax_->ClickEvent.AddHandler(MakeDelegate(this, &NWindow::OnBtnMaxClickedChanged));
            }

            btnSysClose_ = sysButtonGroup->GetChildById<NFrame*>(_NUI_SYS_CLOSE_BUTTON_ID_);
            if(btnSysClose_ != NULL)
            {
                btnSysClose_->ClickEvent.AddHandler(MakeDelegate(this, &NWindow::OnBtnCloseClickedChanged));
            }
        }

        bool NWindow::OnBtnMinClickedChanged(Base::NBaseObj*, NEventData*)
        {
            ::ShowWindow(window_, SW_MINIMIZE);
            return false;
        }

        bool NWindow::OnBtnMaxClickedChanged(Base::NBaseObj*, NEventData*)
        {
            if(::IsZoomed(window_))
                ::ShowWindow(window_, SW_RESTORE);
            else
                ::ShowWindow(window_, SW_MAXIMIZE);
            return false;
        }

        bool NWindow::OnBtnCloseClickedChanged(Base::NBaseObj*, NEventData*)
        {
            Destroy();
            return false;
        }

        HWND NWindow::GetTooltipWnd()
        {
            if(tooltipWnd_ != NULL)
                return tooltipWnd_;

            tooltipWnd_ = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
                WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT,
                CW_USEDEFAULT, CW_USEDEFAULT, window_, NULL, Data::NModule::GetInst().GetNUIModule(), NULL);
            ::SetWindowPos(tooltipWnd_, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

            LRESULT lResult = ::SendMessage(tooltipWnd_, TTM_SETMAXTIPWIDTH, 0, 400);
            // lResult = ::SendMessage(tooltipWnd_, TTM_SETDELAYTIME, TTDT_INITIAL, 300);
            UNREFERENCED_PARAMETER(lResult);

            UpdateTooltipWnd();
            return tooltipWnd_;
        }

        void NWindow::UpdateTooltipWnd()
        {
            if(tooltipWnd_ == NULL)
                return;

            RECT rcClient;
            ::GetClientRect(window_, &rcClient);

            TOOLINFO toolInfo = {0};
            toolInfo.hwnd = window_;
            toolInfo.uId = 1;
            toolInfo.cbSize = sizeof(TOOLINFO);
            LRESULT lResult = ::SendMessage(tooltipWnd_, TTM_DELTOOL, 0, (LPARAM)(&toolInfo));

            TCHAR TempText[2] = {0};
            toolInfo.uFlags = TTF_SUBCLASS;
            toolInfo.rect = rcClient;
            toolInfo.lpszText = TempText;
            lResult = ::SendMessage(tooltipWnd_, TTM_ADDTOOL, 0, (LPARAM)(&toolInfo));
        }

        void NWindow::DestroyTooltipWnd()
        {
            if(tooltipWnd_ != NULL)
            {
                ::DestroyWindow(tooltipWnd_);
                tooltipWnd_ = NULL;
            }
        }

        void NWindow::UpdateTooltipText(const Base::NString& tooltip)
        {
            if(tooltipWnd_ == NULL)
                return;

            TOOLINFO ToolInfo;
            memset(&ToolInfo,0,sizeof(TOOLINFO));
            ToolInfo.cbSize = sizeof(TOOLINFO);
            ToolInfo.hwnd   = window_;
            ToolInfo.uId    = 1;
            ToolInfo.lpszText = (LPTSTR)tooltip.GetData();
            LRESULT lResult = ::SendMessage(tooltipWnd_, TTM_UPDATETIPTEXT, 0, (LPARAM)&ToolInfo);
            UNREFERENCED_PARAMETER(lResult);
        }

        void NWindow::ShowTooltip(const Base::NString& tooltip)
        {
            HWND tooltipWnd_ = GetTooltipWnd();
            if(tooltipWnd_ == NULL)
                return;

            if(!tooltip.IsEmpty())
            {
                UpdateTooltipText(tooltip.GetData());
            }

            // SetWindowPos(tooltipWnd_, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
            LRESULT lResult = ::SendMessage(tooltipWnd_, TTM_ACTIVATE, TRUE, 0);
            UNREFERENCED_PARAMETER(lResult);

            // sometimes, tooltip only show after moving mouse
            //  so we mock mouse event, to show tooltip
            POINT Pt;
            ::GetCursorPos(&Pt);
            ::ScreenToClient(window_, &Pt);
            Pt.x -= 1;
            ::PostMessage(window_, WM_MOUSEMOVE, 0, MAKELPARAM(Pt.x, Pt.y));
            Pt.x += 1;
            ::PostMessage(window_, WM_MOUSEMOVE, 0, MAKELPARAM(Pt.x, Pt.y));
            // ::ShowWindow(tooltipWnd_, SW_SHOWNOACTIVATE);
        }

        void NWindow::HideTooltip()
        {
            if(tooltipWnd_ != NULL)
            {
                ::SendMessage(tooltipWnd_, TTM_ACTIVATE, FALSE, 0);
            }
        }
    }
}
