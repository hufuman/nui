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
            sizableBorder_.SetRect(4, 4, 4, 4);
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
            case WM_CREATE:
                render_ = NUiBus::Instance().CreateRender();
                break;
            case WM_DESTROY:
                render_ = NULL;
                rootFrame_ = NULL;
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
                        hoverFrame_->OnMouseDown(static_cast<short>(point.X), static_cast<short>(point.Y));
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
                        hoverFrame_->OnClicked(point);
                    }
                    NUiBus::Instance().SetCaptureFrame(NULL);
                    ::ReleaseCapture();
                }
                break;
            case WM_MOUSELEAVE:
                SetHoverItem(NULL);
                break;
            }
            return false;
        }

        bool NWindow::OnWndCmd(WPARAM wParam, LPARAM lParam)
        {
            if(wParam == NULL || !::IsWindow(reinterpret_cast<HWND>(lParam)))
                return false;
            NNative* wndUi = NNative::GetWndUi(reinterpret_cast<HWND>(lParam));
            if(wndUi == NULL)
                return false;
            return wndUi->OnParentCommand(HIWORD(wParam));
        }

        void NWindow::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            if(!styleName_.IsEmpty())
                GetRootFrame()->ApplyStyle(styleName_);
#endif  // _NO_NUI_PARSER_
            WindowCreatedEvent.Invoke(this, NULL);
        }

        void NWindow::OnSize(int width, int height)
        {
            if(rootFrame_)
                rootFrame_->SetSize(width, height);

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
            }
        }

        NFrame* NWindow::RefreshHoverItem(const Base::NPoint& point)
        {
            if(rootFrame_ == NULL)
                return NULL;
            NFrame* newHover = NULL;
            if(hoverFrame_)
            {
                newHover = dynamic_cast<NFrame*>(hoverFrame_->GetChildByPointAndFlag(point, NFrameBase::FlagCanHover));
            }
            if(!newHover)
            {
                newHover = dynamic_cast<NFrame*>(rootFrame_->GetChildByPointAndFlag(point, NFrameBase::FlagCanHover));
            }
            return newHover;
        }

        bool NWindow::OnRootFrameSizeChanged(Base::NBaseObj*, NEventData* eventData)
        {
            NFrameBase::SizeEventData* data = static_cast<NFrameBase::SizeEventData*>(eventData);
            SetSize(data->width, data->height);
            return true;
        }
    }
}
