#include "stdafx.h"

#include "../NFrame.h"
#include "../NWindow.h"


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

        NFrame* NWindow::GetRootFrame()
        {
            if(rootFrame_ == NULL)
            {
                Base::NInstPtr<NFrame> rootFrame(MemToolParam);
                rootFrame_ = (NFrame*)rootFrame;
                rootFrame_->SetId(_T("rootFrame"));
                rootFrame_->window_ = this;
                rootFrame->SetLayout(NFrameBase::LayoutHFill | NFrameBase::LayoutVFill);
                Base::NRect rect;
                GetRect(rect);
                rootFrame_->SetSize(rect.Width(), rect.Height());
            }
            return rootFrame_;
        }

        NRender* NWindow::GetRender() const
        {
            return render_;
        }

        void NWindow::SetDrawCallback(WindowDrawCallback callback)
        {
            drawCallback_ = callback;
        }

        // WindowMsgFilter
        bool NWindow::OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
        {
            if(NWindowBase::OnMessage(message, wParam, lParam, lResult))
                return true;

            switch(message)
            {
            case WM_COMMAND:
                if(OnWndCmd(wParam))
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
                    return true;
                }
            case WM_NCPAINT:
            case WM_NCCALCSIZE:
                {
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
                    Base::NPoint point(LOWORD(lParam), HIWORD(lParam));
                    RefreshHoverItem(point);
                }
                break;
            case WM_LBUTTONDOWN:
                {
                    Base::NPoint point(LOWORD(lParam), HIWORD(lParam));
                    RefreshHoverItem(point);
                    if(hoverFrame_ == NULL)
                    {
                        ::SendMessage(window_, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
                    }
                    else
                    {
                        ::SetCapture(window_);
                    }
                }
                break;
            case WM_LBUTTONUP:
                if(::GetCapture() == window_)
                {
                    ::ReleaseCapture();
                    if(hoverFrame_ != NULL)
                    {
                        Base::NPoint point(LOWORD(lParam), HIWORD(lParam));
                        hoverFrame_->OnClicked(point);
                        if(hoverFrame_)
                            hoverFrame_->UpdateStatus(NFrameBase::StatusPressed, false);
                    }
                }
                break;
            case WM_MOUSELEAVE:
                SetHoverItem(NULL);
                break;
            }
            return false;
        }

        bool NWindow::OnWndCmd(WPARAM wParam)
        {
            if(wParam == NULL || !::IsWindow(reinterpret_cast<HWND>(wParam)))
                return false;
            NWndUi* wndUi = NWndUi::GetWndUi(reinterpret_cast<HWND>(wParam));
            if(wndUi == NULL)
                return false;
            return wndUi->OnParentCommand(HIWORD(wParam));
        }

        void NWindow::OnSize(int width, int height)
        {
            if(rootFrame_)
                rootFrame_->SetSize(width, height);

            HRGN rgn = ::CreateRectRgn(0, 0, width, height);
            if(rgn != NULL)
                ::SetWindowRgn(GetNative(), rgn, FALSE);
        }

        void NWindow::OnDraw(NRender* render, HRGN clipRgn)
        {
            Base::NRect rcClient;
            GetRect(rcClient);
            rcClient.Offset(-rcClient.Left, -rcClient.Top);

            if(drawCallback_ && drawCallback_(this, render, clipRgn))
            {
                return;
            }

            if(rootFrame_ != NULL)
            {
                Base::NPoint pt;
                rootFrame_->Draw(render, pt, clipRgn);
            }
        }

        void NWindow::Draw(HDC hDc)
        {
            renderStatus_.BeforeDraw(window_);

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

            renderStatus_.DrawStatus();
            render_->DrawBack(IsLayered());
            ::DeleteObject(clipRgn);

        }

        void NWindow::SetHoverItem(NFrame* frame)
        {
            if(hoverFrame_ && hoverFrame_ != frame)
                hoverFrame_->UpdateStatus(NFrameBase::StatusHover | NFrameBase::StatusPressed, false);
            hoverFrame_ = frame;
            if(hoverFrame_)
                hoverFrame_->UpdateStatus((Util::Shell::IsKeyPressed(VK_LBUTTON) ? NFrameBase::StatusPressed : NFrameBase::StatusHover), true);
        }

        void NWindow::RefreshHoverItem(const Base::NPoint& point)
        {
            if(rootFrame_ == NULL)
                return;
            NFrame* newHover = NULL;
            if(hoverFrame_)
            {
                newHover = dynamic_cast<NFrame*>(hoverFrame_->GetChildByPointAndFlag(point, NFrameBase::FlagCanHover));
            }
            if(!newHover)
            {
                newHover = dynamic_cast<NFrame*>(rootFrame_->GetChildByPointAndFlag(point, NFrameBase::FlagCanHover));
            }
            SetHoverItem(newHover);
        }
    }
}
