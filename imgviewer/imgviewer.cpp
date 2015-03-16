#include "stdafx.h"
#include "imgviewer.h"

#include "ViewerData.h"


namespace
{
    const int g_MinWidth = 400;
    const int g_MinHeight = 300;
    const DWORD g_MagicNum = 0xF74F2AFB;
}

ImgViewer::ImgViewer()
{
    image_ = NULL;
    frameIndex_ = 0;
    frameCount_ = 0;

    supportedForamts_.AddItem(_T(".bmp"));
    supportedForamts_.AddItem(_T(".ico"));
    supportedForamts_.AddItem(_T(".png"));
    supportedForamts_.AddItem(_T(".jpg"));
    supportedForamts_.AddItem(_T(".jpeg"));
    supportedForamts_.AddItem(_T(".gif"));
}

ImgViewer::~ImgViewer()
{
    ;
}

void ImgViewer::Show(LPCTSTR filePath)
{
    nui::Base::NReflect::GetInstance().Create(timerSrv_, MemToolParam);

    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    text_ = loader->CreateText(_T("Double click me Or drag image on me"), MemToolParam);
    font_ = loader->CreateFont(24, MemToolParam);
    font_->SetBold(true);

    text_->SetHorzCenter(true)
        .SetVertCenter(TRUE)
        .SetSingleLine(TRUE)
        .SetColor(MakeArgb(255, 255, 255, 0));

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

    if(filePath != NULL && filePath[0] != 0)
        OpenImage(filePath);
    else
        ViewerDataMgr::Instance().Hold(_T(""), window_.GetNative(), TRUE);
}

void ImgViewer::Destroy()
{
    image_ = NULL;
    timerSrv_ = NULL;
    drawTimerHolder_.Release();
}

const NArrayT<NString>& ImgViewer::GetSupportedFormats() const
{
    return supportedForamts_;
}

HWND ImgViewer::GetNative() const
{
    return window_.GetNative();
}

void ImgViewer::ShowPrev(HWND hWnd, LPCTSTR filePath)
{
    COPYDATASTRUCT cds = {0};
    cds.dwData = g_MagicNum;
    cds.cbData = (_tcslen(filePath) + 1) * 2;
    cds.lpData = (LPBYTE)(LPCTSTR)filePath;
    ::SendMessage(hWnd, WM_COPYDATA, NULL, (LPARAM)&cds);
}

bool ImgViewer::MsgCallback(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);
    UNREFERENCED_PARAMETER(lResult);
    if(message == WM_COPYDATA)
    {
        COPYDATASTRUCT* cds = reinterpret_cast<COPYDATASTRUCT*>(lParam);
        if(cds->dwData == g_MagicNum && cds->cbData % 2 == 0)
        {
            HWND hWnd = window_.GetNative();
            ::ShowWindow(hWnd, SW_SHOW);
            if(::IsIconic(hWnd) || ::GetForegroundWindow() != hWnd)
                ::SwitchToThisWindow(hWnd, TRUE);

            LPCTSTR filePath = reinterpret_cast<LPCTSTR>(cds->lpData);
            if(filePath != NULL && filePath[0] != 0 && File::IsFileExists(filePath))
                OpenImage(filePath);
        }
    }
    else if(message == WM_GETDLGCODE)
    {
        lResult = DLGC_WANTALLKEYS;
        return true;
    }
    else if(message == WM_KEYDOWN)
    {
        if(wParam == VK_ESCAPE && ::GetForegroundWindow() == window_.GetNative())
        {
            window_.Destroy();
        }
    }
    else if(message == WM_NCLBUTTONDBLCLK)
    {
        Base::NString filePath = Shell::BrowseForFile(window->GetNative(), TRUE, GetFileDlgExts());
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
        info->ptMinTrackSize.x = g_MinWidth;
        info->ptMinTrackSize.y = g_MinHeight;
    }
    return false;
}

bool ImgViewer::DrawCallback(NWindow*, NRender* render, const NRect& clipRect)
{
    NRect rcWnd;
    window_.GetRect(rcWnd);
    rcWnd.Offset(-rcWnd.Left, -rcWnd.Top);
    render->FillRectangle(rcWnd, MakeArgb(180, 125, 125, 125));
    if(image_ == NULL)
    {
        render->DrawText(text_, font_, rcWnd);
    }
    else
    {
        NSize size = image_->GetSize();
        if(size.Width < rcWnd.Width() || size.Height < rcWnd.Height())
        {
            rcWnd.GetLeftTop().SetPoint((rcWnd.Width() - size.Width) / 2, (rcWnd.Height() - size.Height) / 2);
            rcWnd.SetSize(size.Width, size.Height);
            render->DrawImage(image_, 0, 0, rcWnd, frameIndex_);
        }
        else
        {
            render->DrawImage(image_, 0, 0, rcWnd, frameIndex_);
        }
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

bool ImgViewer::OpenImage(LPCTSTR filePath)
{
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    NAutoPtr<NImage> image = loader->LoadImage(filePath);
    if(image == NULL)
    {
        ::MessageBox(window_.GetNative(), _T("Failed to open image"), _T("Error"), MB_OK | MB_ICONERROR);
        return false;
    }

    ViewerDataMgr::Instance().Hold(filePath, window_.GetNative(), TRUE);
    image_ = image;
    frameIndex_ = 0;
    frameCount_ = image_->GetFrameCount();
    drawTimerHolder_.Release();
    if(frameCount_ > 1)
    {
        drawTimerHolder_ = timerSrv_->startTimer(image_->NextDelayValue(frameIndex_), MakeDelegate(this, &ImgViewer::DrawTimerFunc));
    }

    NSize size = image_->GetSize();
    window_.SetSize(size.Width, size.Height);
    window_.SetVisible(TRUE);
    window_.Invalidate();
    return true;
}

NString ImgViewer::GetFileDlgExts()
{
    if(!fileDlgExts_.IsEmpty())
        return fileDlgExts_;
    fileDlgExts_ = _T("Image Files|");
    int count = supportedForamts_.Count();
    for(int i=0; i<count; ++ i)
    {
        fileDlgExts_ += _T("*");
        fileDlgExts_ += supportedForamts_[i];
        fileDlgExts_ += _T(";");
    }
    fileDlgExts_ += _T("||");
    return fileDlgExts_;
}