#include "stdafx.h"
#include "../NMsgLoop.h"


namespace nui
{
    namespace Ui
    {
        bool NMsgLoop::Loop()
        {
            HWND window = NULL;
            return Loop(window, false);
        }

        bool NMsgLoop::Loop(HWND window)
        {
            return Loop(window, true);
        }

        bool NMsgLoop::Loop(HWND window, bool useWindow)
        {
            looping_ = true;
            NAssertError(!useWindow || window != NULL, _T("Wrong parameter in NMsgLoop::Loop"));
            MSG msg = {0};
            BOOL bResult = FALSE;
            for(;looping_;)
            {
                // Idle Handle
                if(!::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
                {
                    DoIdleHandler(0);
                }

                bResult = ::GetMessage(&msg, NULL, 0, 0);
                if(bResult <= 0)
                    break;

                if(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST
                    || msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
                {
                    if(::IsDialogMessage(::GetAncestor(msg.hwnd, GA_ROOT), &msg))
                        continue;
                }

                ::TranslateMessage(&msg);
                ::DispatchMessage(&msg);

                if(useWindow && !::IsWindow(window))
                    break;
            }
            if(msg.message == WM_QUIT)
            {
                ::PostQuitMessage(msg.wParam);
            }
            return bResult >= 0;
        }

        void NMsgLoop::Stop()
        {
            looping_ = false;
            ::PostThreadMessage(::GetCurrentThreadId(), WM_NULL, 0, 0);
        }

        bool NMsgLoop::AddIdleHandler(NIdleHandler handler)
        {
            return idleHandlers_.AddItem(handler);
        }

        bool NMsgLoop::RemoveIdleHandler(NIdleHandler handler)
        {
            int pos = idleHandlers_.Find(handler);
            if(pos < 0)
                return false;
            return idleHandlers_.RemoveItem(handler);
        }

        bool NMsgLoop::DoIdleHandler(int idleCount)
        {
            int count = idleHandlers_.Count();
            for(int i=0; i<count; ++ i)
            {
                idleHandlers_[i](idleCount);
            }
            return false;
        }
    }
}
