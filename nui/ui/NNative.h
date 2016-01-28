#pragma once


namespace nui
{
    namespace Ui
    {
        class NUI_CLASS NWindow;

        class NUI_CLASS NNative : public NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("native"))
            friend class NWindow;
        public:
            NNative();
            virtual ~NNative();

            virtual HWND GetNative() const;
            virtual void Attach(NFrame* parentFrame, HWND hwndNative);

            static NNative* GetNativeUi(HWND hWnd);

        protected:
            virtual void OnCreate();

            virtual void OnFocus();
            virtual void OnLostFocus();

            bool IsWndValid() const;

            virtual bool GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle);
            virtual bool OnParentCommand(WORD notifyCode);

            virtual void OnRealWindowCreated();
            void AttachWnd(HWND hWnd);
            void CreateRealWindow();

        protected:
            HWND realWindow_;
        };

    }
}