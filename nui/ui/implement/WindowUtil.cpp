#include "stdafx.h"
#include "WindowUtil.h"

#include "../../data/NModule.h"

namespace nui
{
    namespace UI
    {
        namespace Util
        {
            bool RegisterWindowClass(LPCTSTR szClassName, WNDPROC wndProc)
            {
                WNDCLASSEX wce = {0};
                wce.cbSize = sizeof(wce);
                wce.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
                wce.lpfnWndProc = wndProc;
                wce.hInstance = nui::Data::NModule::GetInst().GetNUIModule();
                wce.hCursor = ::LoadCursor(NULL, IDC_ARROW);
                wce.lpszClassName = szClassName;
                ATOM atom = ::RegisterClassEx(&wce);
                return (atom > 0);
            }

            bool EnsureWindowClass(LPCTSTR szClassName, WNDPROC wndProc)
            {
                WNDCLASSEX wce = {sizeof(WNDCLASSEX)};
                if(::GetClassInfoEx(nui::Data::NModule::GetInst().GetNUIModule(), szClassName, &wce))
                    return true;
                return RegisterWindowClass(szClassName, wndProc);
            }
        }
    }
}
