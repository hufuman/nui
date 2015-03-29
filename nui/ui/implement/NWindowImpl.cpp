#include "stdafx.h"
#include "../NWindow.h"


namespace nui
{
    namespace Ui
    {
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
                    lResult = HTCAPTION;
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
    }
}
