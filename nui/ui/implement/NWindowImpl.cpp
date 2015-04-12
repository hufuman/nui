#include "stdafx.h"
#include "../NWindow.h"


namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NWindow);

        NWindow::NWindow()
        {
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
                    return true;
                }
            case WM_NCPAINT:
            case WM_NCCALCSIZE:
                {
                    lResult = 0;
                    return true;
                }
            case WM_PAINT:
                {
                    PAINTSTRUCT ps = {0};
                    HDC hDc = ::BeginPaint(window_, &ps);
                    Draw(hDc);
                    ::EndPaint(window_, &ps);
                }
                break;
            case WM_PRINT:
                {
                    HDC hDc = (HDC)wParam;
                    Draw(hDc);
                }
                break;
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
                ::SendMessage(window_, WM_SYSCOMMAND, SC_MOVE | HTCAPTION, 0);
                break;
            case WM_LBUTTONUP:
                break;
            case WM_MOUSELEAVE:
                SetHoverItem(NULL);
                break;
            }
            return false;
        }

        void NWindow::OnSize(int width, int height)
        {
            if(rootFrame_)
                rootFrame_->SetSize(width, height);

            HRGN rgn = ::CreateRectRgn(0, 0, width, height);
            if(rgn != NULL)
                ::SetWindowRgn(GetNative(), rgn, FALSE);
        }

        void NWindow::OnDraw(NRender* render, const Base::NRect& clipRect)
        {
            if(drawCallback_ && drawCallback_(this, render, clipRect))
                return;

            if(rootFrame_ != NULL)
            {
                Base::NPoint pt;
                rootFrame_->Draw(render, pt, clipRect);
            }
        }

        void NWindow::Draw(HDC hDc)
        {
            Base::NRect clientRect;
            ::GetClientRect(window_, clientRect);
            render_->Init(hDc, clientRect);

            Base::NRect clipRect;
            int nResult = GetClipBox(hDc, clipRect);
            if(nResult == NULLREGION)
                ::GetClientRect(window_, clipRect);
            OnDraw(render_, clipRect);

            render_->DrawBack(IsLayered());
        }

        void NWindow::SetHoverItem(NFrame* frame)
        {
            if(hoverFrame_ == frame)
                return;
            if(hoverFrame_)
                hoverFrame_->CancelHover();
            hoverFrame_ = frame;
            if(hoverFrame_)
                hoverFrame_->BeginHover();
        }

        void NWindow::RefreshHoverItem(const Base::NPoint& point)
        {
            if(rootFrame_ == NULL)
                return;
            NFrame* newHover = NULL;
            if(hoverFrame_)
            {
                newHover = hoverFrame_->GetChildByPointAndFlag(point, NFrame::FlagCanHover);
            }
            if(!newHover)
            {
                newHover = rootFrame_->GetChildByPointAndFlag(point, NFrame::FlagCanHover);
            }
            SetHoverItem(newHover);
        }
    }
}
