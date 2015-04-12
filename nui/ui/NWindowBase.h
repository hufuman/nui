#pragma once




namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NWindowBase;
        typedef FastDelegate5<NWindowBase*, UINT, WPARAM, LPARAM, LRESULT&, bool> MsgFilterCallback;

        class WindowStyle
        {
        public:
            enum Value
            {
                Top         = 0x0001,
                Child       = 0x0002,
                Transparent = 0x0004,
                Layered     = 0x0008,
                Sizable     = 0x0010,
            };
        };

        class NUI_CLASS NWindowBase : public nui::Base::Noncopyable
        {
        public:
            NWindowBase();
            ~NWindowBase();

            void SetMsgFilterCallback(MsgFilterCallback callback);
            bool Create(HWND parentWindow);
            bool Create(HWND parentWindow, DWORD styleValue);
            bool DoModal(HWND parentWindow);
            void Destroy();
            void SetVisible(BOOL visible);
            void SetText(LPCTSTR text);

            bool GetRect(nui::Base::NRect& rect);
            void SetSize(int width, int height);
            void CenterWindow(HWND relativeWindow);
            void SetRect(const Base::NRect& rect);

            void Invalidate();
            void InvalidateRect(const Base::NRect& rect);

            HWND GetNative() const;

            LRESULT DoDefault(UINT message, WPARAM wParam, LPARAM lParam);

        protected:
            static LRESULT WINAPI _staticWndProc(HWND window, UINT message, WPARAM wParam, LPARAM lParam);
            virtual bool OnMessage(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
            virtual void OnCreate();

            bool IsLayered() const;
            void GetStyle(DWORD styleValue, DWORD& style, DWORD& exStyle) const;

        protected:
            HWND    window_;
            bool    layered_;
            bool    mouseTracking_;
            MsgFilterCallback msgFilterCallback_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}