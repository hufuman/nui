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

            // NFrame
            // data
            virtual void SetText(const Base::NString& text);
            virtual Base::NString GetText() const;
            virtual NText* GetRichText();

        protected:
            virtual bool SetPosImpl(int left, int top, bool force);
            virtual bool SetSizeImpl(int width, int height, bool force);

            bool IsWndValid() const;

            virtual bool GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle) = 0;
            virtual bool OnParentCommand(WORD notifyCode) = 0;

            virtual void OnWindowChanged(NWindow* window);

            void AttachWnd(HWND hWnd);

        protected:
            HWND realWindow_;
        };

    }
}