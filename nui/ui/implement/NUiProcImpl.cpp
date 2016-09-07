#include "StdAfx.h"
#include "NUiProcImpl.h"

namespace nui
{
    namespace Ui
    {
        namespace Impl
        {
            DWORD g_UiProcMsg = ::RegisterWindowMessage(_T("_nui_ui_proc_msg_"));

            NUiProc::NUiProc()
            {
            }

            NUiProc::~NUiProc()
            {
                Destroy();
            }

            NUiProc& NUiProc::Get()
            {
                static NUiProc instance;
                return instance;
            }

            void NUiProc::Init()
            {
                window_.Create(HWND_MESSAGE);
                NAssertError(window_.GetNative() != NULL, _T("Failed to create window"));
                window_.SetMsgFilterCallback(MakeDelegate(this, &NUiProc::WindowProc));
            }

            void NUiProc::Destroy()
            {
                if(window_.GetNative() != NULL)
                    window_.Destroy();
            }

            void NUiProc::PostUiProc(const NUiProcHandler& handler, void* data)
            {
                NUiProcHandler* tempHandler = new NUiProcHandler;
                *tempHandler = handler;
                ::PostMessage(window_.GetNative(), g_UiProcMsg, reinterpret_cast<WPARAM>(tempHandler), reinterpret_cast<LPARAM>(data));
            }

            LRESULT NUiProc::SendUiProc(const NUiProcHandler& handler, void* data)
            {
                NUiProcHandler* tempHandler = new NUiProcHandler;
                *tempHandler = handler;
                return ::SendMessage(window_.GetNative(), g_UiProcMsg, reinterpret_cast<WPARAM>(tempHandler), reinterpret_cast<LPARAM>(data));
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
        }

        void PostUiProc(NUiProcHandler handler, void* data)
        {
            nui::Ui::Impl::NUiProc::Get().PostUiProc(handler, data);
        }

        LRESULT SendUiProc(NUiProcHandler handler, void* data)
        {
            return nui::Ui::Impl::NUiProc::Get().SendUiProc(handler, data);
        }
    }
}
