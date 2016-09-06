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
            virtual Base::NSize GetAutoSize() const;

            virtual void SetText(const Base::NString& text);
            virtual Base::NString GetText() const;

        protected:
            virtual void OnCreate();

            virtual void OnSizeChanged(int width, int height);
            virtual void OnPosChanged(int left, int top);

            virtual void OnFocus();
            virtual void OnLostFocus();

            bool IsWndValid() const;

            virtual bool GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle);
            virtual bool OnParentCommand(WORD notifyCode);

            virtual void OnRealWindowCreated();
            void AttachWnd(HWND hWnd);
            void CreateRealWindow();

            Base::NString GetNativeText() const;

        protected:
            HWND realWindow_;
        };

    }
}