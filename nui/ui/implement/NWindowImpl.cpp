#include "stdafx.h"
#include "../NWindow.h"


namespace nui
{
    namespace Ui
    {
        NWindow::NWindow()
        {
            ;
        }

        NWindow::~NWindow()
        {
            ;
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
                    HDC hDC = ::BeginPaint(window_, &ps);

                    Base::NRect clientRect;
                    ::GetClientRect(window_, clientRect);
                    render_->Init(hDC, clientRect);

                    Base::NRect clipRect;
                    int nResult = GetClipBox(hDC, clipRect);
                    if(nResult == NULLREGION)
                        ::GetClientRect(window_, clipRect);
                    OnDraw(render_, clipRect);

                    render_->DrawBack();
                    ::EndPaint(window_, &ps);
                }
                break;
            case WM_PRINT:
                {
                    HDC hDC = (HDC)wParam;

                    Base::NRect clientRect;
                    ::GetClientRect(window_, clientRect);
                    render_->Init(hDC, clientRect);

                    Base::NRect clipRect;
                    int nResult = GetClipBox(hDC, clipRect);
                    if(nResult == NULLREGION)
                        ::GetClientRect(window_, clipRect);
                    OnDraw(render_, clipRect);

                    render_->DrawBack();
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
            HRGN rgn = ::CreateRectRgn(0, 0, width, height);
            if(rgn != NULL)
                ::SetWindowRgn(GetNative(), rgn, FALSE);
        }

        void NWindow::OnDraw(NRender* render, const Base::NRect& clipRect)
        {
            // test
            render->FillRectangle(clipRect, Ui::MakeArgb(255, 255, 255, 0));
        }
    }
}
