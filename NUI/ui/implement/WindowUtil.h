#pragma once



namespace NUI
{
    namespace UI
    {
        namespace Util
        {
            bool RegisterWindowClass(LPCTSTR szClassName, WNDPROC wndProc);
            bool EnsureWindowClass(LPCTSTR szClassName, WNDPROC wndProc);
        }
    }
}
