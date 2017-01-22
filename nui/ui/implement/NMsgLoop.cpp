#include "stdafx.h"
#include "../NMsgLoop.h"


namespace nui
{
    namespace Ui
    {
		IMPLEMENT_REFLECTION(NMsgLoop);
		
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
            TCHAR className[50];
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

                // for edit: ctrl+A
                if(msg.message == WM_KEYDOWN && msg.wParam == 'A' && ::GetKeyState(VK_CONTROL) < 0)
                {
                    HWND hFocused = ::GetFocus();
                    if(hFocused != NULL)
                    {
                        if(::GetClassName(hFocused, className, _countof(className)) > 0 && _tcsicmp(className, WC_EDIT) == 0)
                        {
                            ::SendMessage(hFocused, EM_SETSEL, 0, -1);
                            continue;
                        }
                    }
                }

                //if(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST
                //    || msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
                //{
                //    if(::IsDialogMessage(::GetAncestor(msg.hwnd, GA_ROOT), &msg))
                //        continue;
                //}

				if (!DoMessageFilter(&msg))
				{
					::TranslateMessage(&msg);
					::DispatchMessage(&msg);
				}

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

		bool NMsgLoop::AddMessageFilter(NMessageFilter filter)
		{
			return messageFilters_.AddItem(filter);
		}

		bool NMsgLoop::RemoveMessageFilter(NMessageFilter filter)
		{
			int pos = messageFilters_.Find(filter);
			if (pos < 0)
				return false;
			return messageFilters_.RemoveItem(filter);
		}

		bool NMsgLoop::DoMessageFilter(MSG* msg)
		{
			int count = messageFilters_.Count();
			for (int i = 0; i<count; ++i)
			{
				if (messageFilters_[i](msg))
					return true;
			}
			return false;
		}
    }
}
