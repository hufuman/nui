#include "StdAfx.h"
#include "../NUiProc.h"

namespace nui
{
    namespace Ui
    {
        namespace
        {
            DWORD g_UiProcMsg = ::RegisterWindowMessage(_T("_nui_ui_proc_msg_"));

            class NUiProc
            {
            public:
                NUiProc(void);
                ~NUiProc(void);

            public:
                HWND GetMsgWnd();

                bool WindowProc(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

            private:

                NWindowBase window_;
            };

            NUiProc::NUiProc(void)
            {
            }

            NUiProc::~NUiProc(void)
            {
                if(window_.GetNative() != NULL)
                    window_.Destroy();
            }

            HWND NUiProc::GetMsgWnd()
            {
                if(window_.GetNative() != NULL)
                    return window_.GetNative();
                window_.Create(HWND_MESSAGE);
                NAssertError(window_.GetNative() != NULL, _T("Failed to create window"));
                window_.SetMsgFilterCallback(MakeDelegate(this, &NUiProc::WindowProc));
                return window_.GetNative();
            }

            bool NUiProc::WindowProc(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
            {
                UNREFERENCED_PARAMETER(window);
                UNREFERENCED_PARAMETER(wParam);
                UNREFERENCED_PARAMETER(lParam);
                UNREFERENCED_PARAMETER(lResult);
                if(message != g_UiProcMsg)
                    return false;

                NUiProcHandler* handler = reinterpret_cast<NUiProcHandler*>(wParam);
                void* data = reinterpret_cast<void*>(lParam);
                lResult = (*handler)(data);

                delete handler;
                return true;
            }

            NUiProc g_UiProc;
        }

        void PostUiProc(NUiProcHandler handler, void* data)
        {
            NUiProcHandler* tempHandler = new NUiProcHandler;
            *tempHandler = handler;
            ::PostMessage(g_UiProc.GetMsgWnd(), g_UiProcMsg, reinterpret_cast<WPARAM>(tempHandler), reinterpret_cast<LPARAM>(data));
        }

        LRESULT SendUiProc(NUiProcHandler handler, void* data)
        {
            NUiProcHandler* tempHandler = new NUiProcHandler;
            *tempHandler = handler;
            return ::SendMessage(g_UiProc.GetMsgWnd(), g_UiProcMsg, reinterpret_cast<WPARAM>(tempHandler), reinterpret_cast<LPARAM>(data));
        }
    }
}
