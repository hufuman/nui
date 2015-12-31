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

            virtual void Attach(NFrame* parentFrame, HWND hwndNative);
            virtual HWND GetNative() const;

            static NNative* GetWndUi(HWND hWnd);

        public:
            // NFrame
            // data
            virtual void SetText(const Base::NString& text);
            virtual Base::NString GetText() const;
            virtual NText* GetRichText();

            // flags
            virtual bool SetVisible(bool visible);
            virtual bool SetEnabled(bool enabled);

            virtual Base::NSize GetAutoSize() const;

        protected:

            virtual void OnSizeChanged(int width, int height);
            virtual void OnPosChanged(int left, int top);

            bool IsWndValid() const;

            virtual bool GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle);
            virtual bool OnParentCommand(WORD notifyCode);

            virtual void OnWindowChanged(NWindow* window);

            void AttachWnd(HWND hWnd);

        protected:
            HWND realWindow_;
        };

    }
}