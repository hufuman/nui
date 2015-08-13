#pragma once


namespace nui
{
    namespace Ui
    {

        template < typename TBase >
        class NWndUi : public TBase
        {
        public:
            NWndUi()
            {
                realWindow_ = NULL;
                oldWndProc_ = NULL;
            }
            virtual ~NWndUi(){}

        protected:
            void AttachWnd(HWND hWnd)
            {
                NAssertError(hWnd != NULL && ::IsWindow(hWnd), _T("Not A Good Window"));
                if(hWnd == NULL || !::IsWindow(hWnd))
                    return;
                NAssertError(realWindow_ == NULL && ::GetProp(hWnd, _T("NWndUiObj")) == NULL, _T("NWndUi Attached Already"));
                realWindow_ = hWnd;
                oldWndProc_ = reinterpret_cast<WNDPROC>(::GetWindowLongPtr(realWindow_, GWLP_WNDPROC));
                ::SetProp(realWindow_, _T("NWndUiObj"), reinterpret_cast<HANDLE>(this));
                ::SetWindowLongPtr(realWindow_, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&NWndUi::_WndUiProc));
            }

            static LRESULT WINAPI _WndUiProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
            {
                NWndUi* pThis = reinterpret_cast<NWndUi*>(::GetProp(hWnd, _T("NWndUiObj")));
                if(pThis == NULL)
                    return ::CallWindowProc(&::DefWindowProc, hWnd, message, wParam, lParam);

                LRESULT lResult = false;
                if(pThis->OnWndMessage(message, wParam, lParam, lResult))
                    return lResult;

                return pThis->DoDefault(message, wParam, lParam);
            }

            LRESULT DoDefault(UINT message, WPARAM wParam, LPARAM lParam)
            {
                WNDPROC wndProd = oldWndProc_ == NULL ? ::DefWindowProc : oldWndProc_;
                return ::CallWindowProc(wndProd, realWindow_, message, wParam, lParam);
            }

            virtual bool OnWndMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
            {
                UNREFERENCED_PARAMETER(message);
                UNREFERENCED_PARAMETER(wParam);
                UNREFERENCED_PARAMETER(lParam);
                UNREFERENCED_PARAMETER(lResult);
                return false;
            }

        private:
            WNDPROC oldWndProc_;
            HWND realWindow_;
        };

    }
}