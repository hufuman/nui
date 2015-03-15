#include "stdafx.h"
#include "imgviewer.h"


ImgViewer::ImgViewer()
{
    image_ = NULL;
    frameIndex_ = 0;
    frameCount_ = 0;
}

ImgViewer::~ImgViewer()
{
    ;
}

void ImgViewer::Show()
{
    nui::Base::NReflect::GetInstance().Create(timerSrv_, MemToolParam);

    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    text_ = loader->CreateText(_T("Double click me Or drag image on me"), MemToolParam);
    font_ = loader->CreateFont(24, MemToolParam);

    text_->SetHorzCenter(true)
        .SetVertCenter(TRUE)
        .SetSingleLine(TRUE)
        .SetColor(MakeArgb(255, 123, 123, 123));

    window_.SetDrawCallback(MakeDelegate(this, &ImgViewer::DrawCallback));
    window_.SetMsgFilterCallback(MakeDelegate(this, &ImgViewer::MsgCallback));
    window_.Create(NULL, WindowStyle::Layered | WindowStyle::Sizable);
    window_.SetSize(500, 400);
    window_.CenterWindow(NULL);
    window_.SetText(_T("Test Window"));
    window_.SetVisible(true);

    ::DragAcceptFiles(window_.GetNative(), TRUE);
    Shell::FilterWindowMessage(0x0049 /*WM_COPYGLOBALDATA*/, 1);
    Shell::FilterWindowMessage(WM_DROPFILES, 1);
}

void ImgViewer::Destroy()
{
    image_ = NULL;
    timerSrv_ = NULL;
    drawTimerHolder_.Release();
}

HWND ImgViewer::GetNative() const
{
    return window_.GetNative();
}

bool ImgViewer::MsgCallback(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(lResult);
    if(message == WM_NCLBUTTONDBLCLK)
    {
        Base::NString filePath = Shell::BrowseForFile(window->GetNative(), TRUE, _T("Image Files|*.bmp;*.ico;*.png;*.jpg;*.jpeg;*.gif||"));
        if(filePath.IsEmpty())
            return false;

        OpenImage(filePath);
    }
    else if(message == WM_DROPFILES)
    {
        HDROP hDrop = (HDROP)wParam;
        if(hDrop != NULL)
        {
            UINT uNumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
            if(uNumFiles > 0 && uNumFiles != 0xFFFFFFFF)
            {
                TCHAR buffer[1024];
                if(::DragQueryFile(hDrop, 0, buffer, MAX_PATH) > 0)
                {
                    OpenImage(buffer);
                }
            }
            DragFinish(hDrop);
            return true;
        }
    }
    else if(message == WM_GETMINMAXINFO)
    {
        MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
        info->ptMinTrackSize.x = 10;
        info->ptMinTrackSize.y = 10;
    }
    return false;
}

bool ImgViewer::DrawCallback(NWindow*, NRender* render, const NRect& clipRect)
{
    NRect rcWnd;
    window_.GetRect(rcWnd);
    rcWnd.Offset(-rcWnd.Left, -rcWnd.Top);
    if(image_ == NULL)
    {
        render->FillRectangle(rcWnd, MakeArgb(200, 255, 255, 0));
        render->DrawText(text_, font_, rcWnd);
    }
    else
    {
        render->DrawImage(image_, 0, 0, rcWnd, frameIndex_);
    }
    return true;
}

void ImgViewer::DrawTimerFunc()
{
    frameIndex_ = (frameIndex_ + 1) % frameCount_;
    window_.Invalidate();
    drawTimerHolder_.Release();
    drawTimerHolder_ = timerSrv_->startTimer(image_->NextDelayValue(frameIndex_), MakeDelegate(this, &ImgViewer::DrawTimerFunc));
}

void ImgViewer::OpenImage(LPCTSTR filePath)
{
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    image_ = loader->LoadImage(filePath);

    frameIndex_ = 0;
    frameCount_ = image_->GetFrameCount();
    drawTimerHolder_.Release();
    if(frameCount_ > 1)
    {
        drawTimerHolder_ = timerSrv_->startTimer(image_->NextDelayValue(frameIndex_), MakeDelegate(this, &ImgViewer::DrawTimerFunc));
    }

    NSize size = image_->GetSize();
    window_.SetSize(size.Width, size.Height);
    window_.CenterWindow(NULL);
    window_.SetVisible(TRUE);
    window_.Invalidate();
}
