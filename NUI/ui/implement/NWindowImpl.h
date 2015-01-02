#pragma once



#include "../NWindow.h"
#include "../NWindowBase.h"

namespace NUI
{
    namespace UI
    {
        class NWindowImpl : public NWindow
        {
        public:
            NWindowImpl();
            ~NWindowImpl();

        public:
            // NWindow
            virtual bool Create(HWND parentWindow);
            virtual void Destroy();
            virtual bool DoModal(HWND parentWindow);
            virtual void SetVisible(bool visible);
            virtual void SetPos(int left, int top);
            virtual void SetSize(int width, int height);

        public:
            // WindowMsgFilter
            virtual bool OnMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

        protected:
            NWindowBase window_;
        };
    }
}
