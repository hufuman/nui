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
                MenuLike    = 0x0020 | Top
            };
        };

        class NUI_CLASS NWindowBase : public nui::Base::Noncopyable
        {
        private:
            class WindowBasePrivateData : public nui::Base::NBaseObj
            {
                WindowBasePrivateData(const WindowBasePrivateData&);
                WindowBasePrivateData& operator = (const WindowBasePrivateData&);
            public:
                WindowBasePrivateData()
                {
                    visible = false;
                    centerWindow = true;
                    centerRelativeWindow = NULL;
                }
                Base::NString text;
                Base::NRect rect;
                bool visible;
                bool centerWindow;
                HWND centerRelativeWindow;
            };

        public:
            NWindowBase();
            ~NWindowBase();

            void SetMsgFilterCallback(MsgFilterCallback callback);
            bool Create(HWND parentWindow);
            bool Create(HWND parentWindow, DWORD styleValue);
            bool DoModal(HWND parentWindow);
            void Destroy();

            // WindowStyle
            void SetStyle(DWORD styleValue);
            void SetVisible(bool visible);
            void SetText(LPCTSTR text);

            bool GetRect(nui::Base::NRect& rect);
            void SetSize(int width, int height);
            void SetPos(int x, int y);
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
            virtual void Draw(HDC hDc);

            bool IsRegionEmpty(HRGN clipRgn);
            void ResetInvalidRgn();
            bool IsLayered() const;
            void GetStyle(DWORD& style, DWORD& exStyle) const;

            nui::Base::NAutoPtr<WindowBasePrivateData> GetPrivateData();

        protected:
            HWND    window_;
            UINT    windowStyle_;
            bool    mouseTracking_;
            MsgFilterCallback msgFilterCallback_;

            HRGN    invalidateRgn_;
            DWORD   lastDrawTick_;
            DWORD   drawTimerId_;

            HWND    modalParent_;

            nui::Base::NAutoPtr<WindowBasePrivateData> privateWindowBaseData_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
