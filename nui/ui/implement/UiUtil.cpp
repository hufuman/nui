#include "stdafx.h"
#include "UiUtil.h"

#include "../NNative.h"


namespace UiUtil
{
    enum SyncType
    {
        SyncVisible,
        SyncEnable
    };

    bool IsChild(nui::Ui::NFrameBase* frame, nui::Ui::NFrameBase* root, SyncType syncType, bool& realFlag)
    {
        realFlag = true;
        while(frame != NULL)
        {
            if(syncType == SyncVisible)
                realFlag = realFlag && frame->IsVisible();
            else if(syncType == SyncEnable)
                realFlag = realFlag && frame->IsEnabled();

            if(frame == root)
                return true;
            frame = frame->GetParent();
        }
        return false;
    }

    void SyncNativeWindowStatus(nui::Ui::NFrameBase* frame, SyncType syncType)
    {
        if(frame == NULL)
            return;
        nui::Ui::NWindow* window = frame->GetWindow();
        if(window == NULL)
            return;
        HWND hWnd = ::GetWindow(window->GetNative(), GW_CHILD);
        while(hWnd != NULL)
        {
            nui::Ui::NNative* native = nui::Ui::NNative::GetNativeUi(hWnd);
            if(native)
            {
                bool realFlag = false;
                if(IsChild(native, frame, syncType, realFlag))
                {
                    if(syncType == SyncVisible)
                    {
                        ::ShowWindow(native->GetNative(), realFlag);
                    }
                    else if(syncType == SyncEnable)
                    {
                        ::EnableWindow(native->GetNative(), realFlag);
                    }
                }
            }
            hWnd = ::GetWindow(hWnd, GW_HWNDNEXT);
        }
    }

    // fixme: since real window is created only when focused, these two functions could be deleted?
    void SyncNativeWindowByVisible(nui::Ui::NFrameBase* frame)
    {
        SyncNativeWindowStatus(frame, SyncVisible);
    }

    void SyncNativeWindowByEnable(nui::Ui::NFrameBase* frame)
    {
        SyncNativeWindowStatus(frame, SyncEnable);
    }
}
