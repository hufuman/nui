#include "stdafx.h"
#include "NWindowImpl.h"


namespace NUI
{
    namespace UI
    {
        NWindowImpl::NWindowImpl()
        {
            ;
        }

        NWindowImpl::~NWindowImpl()
        {
            ;
        }

        // NWindow
        bool NWindowImpl::Create(HWND parentWindow)
        {
            UNREFERENCED_PARAMETER(parentWindow);
            return false;
        }

        void NWindowImpl::Destroy()
        {
            ;
        }

        bool NWindowImpl::DoModal(HWND parentWindow)
        {
            UNREFERENCED_PARAMETER(parentWindow);
            return false;
        }

        void NWindowImpl::SetVisible(bool visible)
        {
            UNREFERENCED_PARAMETER(visible);
        }

        void NWindowImpl::SetPos(int left, int top)
        {
            UNREFERENCED_PARAMETER(left);
            UNREFERENCED_PARAMETER(top);
        }

        void NWindowImpl::SetSize(int width, int height)
        {
            UNREFERENCED_PARAMETER(width);
            UNREFERENCED_PARAMETER(height);
        }

        // WindowMsgFilter
        bool NWindowImpl::OnMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
        {
            UNREFERENCED_PARAMETER(window);
            UNREFERENCED_PARAMETER(message);
            UNREFERENCED_PARAMETER(wParam);
            UNREFERENCED_PARAMETER(lParam);
            UNREFERENCED_PARAMETER(lResult);
            return false;
        }

    }
}
