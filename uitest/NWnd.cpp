#include "StdAfx.h"
#include "NWnd.h"

NWnd::NWnd(void)
{
}

NWnd::~NWnd(void)
{
}

bool NWnd::Create(HWND parentWindow)
{
    window_.SetMsgFilterCallback(MakeDelegate(this, &NWnd::WindowProc));
    if(!window_.Create(parentWindow))
        return false;
    return true;
}

void NWnd::SetSize(int width, int height)
{
    window_.SetSize(width, height);
}

void NWnd::CenterWindow(HWND relativeWindow)
{
    window_.CenterWindow(relativeWindow);
}

void NWnd::SetText(LPCTSTR text)
{
    window_.SetText(text);
}

void NWnd::SetVisible(bool visible)
{
    window_.SetVisible(visible);
}

void NWnd::Destroy()
{
    window_.Destroy();
}

HWND NWnd::GetNative() const
{
    return window_.GetNative();
}

void NWnd::SetMsgFilterCallback(nui::Ui::MsgFilterCallback callback)
{
    callback_ = callback;
}

void NWnd::DoPaint()
{
    PAINTSTRUCT ps = {0};
    HDC hDc = ::BeginPaint(window_.GetNative(), &ps);

    ::FillRect(hDc, &ps.rcPaint, (HBRUSH)::GetStockObject(GRAY_BRUSH));

    ::EndPaint(window_.GetNative(), &ps);
}

void NWnd::UpdateWindowCorner(int width, int height)
{
    HRGN rgn = ::CreateRectRgn(0, 0, width, height);
    if(rgn != NULL)
        ::SetWindowRgn(window_.GetNative(), rgn, FALSE);
}

bool NWnd::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    if(callback_ && callback_(hWnd, message, wParam, lParam, lResult))
        return true;
    switch(message)
    {
    case WM_ERASEBKGND:
        lResult = 1;
        return true;
    case WM_PAINT:
        DoPaint();
        return true;
    case WM_NCACTIVATE:
        {
            if(::IsIconic(hWnd))
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
    case WM_SIZE:
        if(wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
        {
            lResult = window_.DoDefault(message, wParam, lParam);
            int width = LOWORD(lParam);
            int height = HIWORD(lParam);
            UpdateWindowCorner(width, height);
            return true;
        }
        break;
    }
    return false;
}
