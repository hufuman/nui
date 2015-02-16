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

        // NWindow
        bool NWindow::Create(HWND parentWindow)
        {
            UNREFERENCED_PARAMETER(parentWindow);
            return false;
        }

        void NWindow::Destroy()
        {
            ;
        }

        bool NWindow::DoModal(HWND parentWindow)
        {
            UNREFERENCED_PARAMETER(parentWindow);
            return false;
        }

        void NWindow::SetVisible(bool visible)
        {
            UNREFERENCED_PARAMETER(visible);
        }

        void NWindow::SetPos(int left, int top)
        {
            UNREFERENCED_PARAMETER(left);
            UNREFERENCED_PARAMETER(top);
        }

        void NWindow::SetSize(int width, int height)
        {
            UNREFERENCED_PARAMETER(width);
            UNREFERENCED_PARAMETER(height);
        }

        void NWindow::SetIcon(LPCTSTR iconPath)
        {
            UNREFERENCED_PARAMETER(iconPath);
        }

        // WindowMsgFilter
        bool NWindow::WindowProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
        {
            UNREFERENCED_PARAMETER(window);
            UNREFERENCED_PARAMETER(message);
            UNREFERENCED_PARAMETER(wParam);
            UNREFERENCED_PARAMETER(lParam);
            UNREFERENCED_PARAMETER(lResult);
            return false;
        }

    }
}
