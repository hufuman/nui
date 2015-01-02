#pragma once




namespace NUI
{
    namespace UI
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
            typedef FastDelegate5<HWND, UINT, WPARAM, LPARAM, LRESULT&, bool> MsgFilterCallback;

        class NUI_CLASS NWindowBase : public NUI::Base::Noncopyable
        {
        public:
            NWindowBase();
            ~NWindowBase();

            void SetMsgFilterCallback(MsgFilterCallback callback);
            bool Create(HWND parentWindow, LPCTSTR szText, NUI::Base::NRect& rect);
            bool DoModal(HWND parentWindow, LPCTSTR szText, NUI::Base::NRect& rect);
            void Destroy();
            void ShowWindow(DWORD showFlag);

            HWND GetNative() const;

        private:
            static LRESULT WINAPI _staticWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
            virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

        protected:
            HWND    window_;
            MsgFilterCallback msgFilterCallback_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}