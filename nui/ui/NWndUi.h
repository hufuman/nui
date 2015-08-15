#pragma once


namespace nui
{
    namespace Ui
    {
        class NUI_CLASS NWindow;

        class NUI_CLASS NWndUi : public NFrame
        {
            friend class NWindow;
        public:
            NWndUi();
            virtual ~NWndUi();

            static NWndUi* GetWndUi(HWND hWnd);

        protected:

            virtual bool GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle) = 0;
            virtual bool OnParentCommand(WORD notifyCode) = 0;

            virtual void OnWindowChanged(NWindow* window);

            void AttachWnd(HWND hWnd);

        private:
            HWND realWindow_;
        };

    }
}