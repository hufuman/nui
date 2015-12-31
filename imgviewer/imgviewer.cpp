#include "stdafx.h"
#include "imgviewer.h"

#include "ViewerData.h"


namespace
{
    const int g_MinWidth = 400;
    const int g_MinHeight = 300;
    const int g_HorzMargin = 100;
    const int g_VertMargin = 90;
    const DWORD g_MagicNum = 0xF74F2AFB;
}

ImgViewer::ImgViewer()
{
    image_ = NULL;
    frameIndex_ = 0;
    frameCount_ = 0;
    rate_ = 1;

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

    text_->SetAlignFlags(NText::TextAlignHCenter | NText::TextAlignVCenter)
        ->SetSingleLine(true)
        ->SetColor(MakeArgb(255, 255, 255, 0));

    NReflectCreate(window_);
    window_->PostDrawEvent.AddHandler(MakeDelegate(this, &ImgViewer::PostDrawCallback));
    window_->SetMsgFilterCallback(MakeDelegate(this, &ImgViewer::MsgCallback));
    window_->Create(NULL, WindowStyle::Layered | WindowStyle::Sizable);
    window_->SetSize(500, 400);
    window_->SetText(_T("Test Window"));

    NShapeDraw* shape = NULL;
    NReflectCreate(shape);
    shape->SetFillColor(MakeArgb(255, 255, 0, 255));

    InitControls();

    ::DragAcceptFiles(window_->GetNative(), TRUE);
    Shell::FilterWindowMessage(0x0049 /*WM_COPYGLOBALDATA*/, 1);
    Shell::FilterWindowMessage(WM_DROPFILES, 1);
    Shell::FilterWindowMessage(WM_COPYDATA, 1);

    if(filePath != NULL && filePath[0] != 0)
    {
        OpenImage(filePath);
    }
    else
    {
        ViewerDataMgr::Instance().Hold(_T(""), window_->GetNative(), TRUE);
        window_->CenterWindow(NULL);
        window_->SetVisible(true);
    }
}

void ImgViewer::Destroy()
{
    image_ = NULL;
    timerSrv_ = NULL;
    drawTimerHolder_.Release();
    window_ = NULL;
}

const NArrayT<NString>& ImgViewer::GetSupportedFormats() const
{
    return supportedForamts_;
}

HWND ImgViewer::GetNative() const
{
    return window_->GetNative();
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
        OnCopyData(cds);
    }
    else if(message == WM_GETDLGCODE)
    {
        lResult = DLGC_WANTALLKEYS;
        return true;
    }
    else if(message == WM_KEYDOWN)
    {
        if(wParam == VK_ESCAPE && ::GetForegroundWindow() == window_->GetNative())
        {
            window_->Destroy();
        }
    }
    else if(message == WM_LBUTTONDBLCLK)
    {
        Base::NString filePath;
        if(!Shell::BrowseForFile(filePath, window->GetNative(), TRUE, GetFileDlgExts()) || filePath.IsEmpty())
            return false;

        OpenImage(filePath);
    }
    else if(message == WM_MOUSEWHEEL)
    {
        OnMouseWheel((short)HIWORD(wParam));
    }
    else if(message == WM_DROPFILES)
    {
        HDROP hDrop = (HDROP)wParam;
        if(hDrop != NULL)
        {
            OnDropFiles(hDrop);
            return true;
        }
    }
    else if(message == WM_GETMINMAXINFO)
    {
        MINMAXINFO* info = reinterpret_cast<MINMAXINFO*>(lParam);
        info->ptMinTrackSize.x = g_MinWidth;
        info->ptMinTrackSize.y = g_MinHeight;
        info->ptMaxTrackSize.x = 65535;
        info->ptMaxTrackSize.y = 65535;
    }
    return false;
}

bool ImgViewer::PostDrawCallback(NBaseObj* baseObj, NEventData* eventData)
{
    NWindow::WindowDrawEventData* data = static_cast<NWindow::WindowDrawEventData*>(eventData);
    NRect rcWnd;
    window_->GetRect(rcWnd);
    rcWnd.Offset(-rcWnd.Left, -rcWnd.Top);
    data->render->FillRectangle(rcWnd, MakeArgb(180, 125, 125, 125));
    if(image_ == NULL)
    {
        NSize size;
        data->render->GetTextSize(text_, font_, size);
        NRect txtRect;
        const int margin = 4;
        txtRect.SetPos((rcWnd.Width() - size.Width) / 2 - margin, (rcWnd.Height() - size.Height) / 2 - margin);
        txtRect.SetSize(size.Width + 2 * margin, size.Height + 2 * margin);
        data->render->FillRoundRectangle(txtRect, margin, MakeArgb(180, 58, 58, 58));
        data->render->DrawText(text_, font_, rcWnd);
    }
    else
    {
        NSize size = image_->GetPreferSize();
        if(rate_ > 0)
        {
            size.Width *= rate_;
            size.Height *= rate_;
        }
        else
        {
            size.Width /= -rate_;
            size.Height /= -rate_;
        }
        if(size.Width < rcWnd.Width() || size.Height < rcWnd.Height())
        {
            rcWnd.GetLeftTop().SetPoint((rcWnd.Width() - size.Width) / 2, (rcWnd.Height() - size.Height) / 2);
            rcWnd.SetSize(size.Width, size.Height);
            data->render->DrawImage(image_, 0, 0, rcWnd, frameIndex_);
        }
        else
        {
            data->render->DrawImage(image_, 0, 0, rcWnd, frameIndex_);
        }
    }
    return false;
}

void ImgViewer::DrawTimerFunc()
{
    frameIndex_ = (frameIndex_ + 1) % frameCount_;
    window_->Invalidate();
    drawTimerHolder_.Release();
    drawTimerHolder_ = timerSrv_->startTimer(image_->NextDelayValue(frameIndex_), MakeDelegate(this, &ImgViewer::DrawTimerFunc));
}

bool ImgViewer::OpenImage(LPCTSTR filePath)
{
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    NAutoPtr<NImageDraw> image = loader->LoadImage(filePath);
    if(image == NULL)
    {
        ::MessageBox(window_->GetNative(), _T("Failed to open image"), _T("Error"), MB_OK | MB_ICONERROR);
        return false;
    }

    ViewerDataMgr::Instance().Hold(filePath, window_->GetNative(), TRUE);
    image_ = image;
    frameIndex_ = 0;
    frameCount_ = image_->GetFrameCount();
    drawTimerHolder_.Release();
    if(frameCount_ > 1)
    {
        drawTimerHolder_ = timerSrv_->startTimer(image_->NextDelayValue(frameIndex_), MakeDelegate(this, &ImgViewer::DrawTimerFunc));
    }

    NRect rcWorkarea = Shell::GetWorkareaRect();
    NSize size = image_->GetPreferSize();
    if(size.Width < rcWorkarea.Width() && size.Height < rcWorkarea.Height())
    {
        rate_ = 1;
    }
    else
    {
        int xRate = size.Width / rcWorkarea.Width() + 2;
        int yRate = size.Height / rcWorkarea.Height() + 2;
        rate_ = xRate < yRate ? - xRate : - yRate;
    }
    GetProperSize(size);
    window_->SetSize(size.Width, size.Height);
    window_->SetVisible(TRUE);
    window_->Invalidate();
    window_->CenterWindow(NULL);
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

void ImgViewer::GetProperSize(NSize& size)
{
    if(rate_ > 0)
    {
        size.Width *= rate_;
        size.Height *= rate_;
    }
    else
    {
        size.Width /= -rate_;
        size.Height /= -rate_;
    }
    size.Width += 2 * g_HorzMargin;
    size.Height += 2 * g_VertMargin;
}

void ImgViewer::OnCopyData(COPYDATASTRUCT* cds)
{
    if(cds->dwData != g_MagicNum || cds->cbData % 2 != 0)
        return;

    HWND hWnd = window_->GetNative();
    ::ShowWindow(hWnd, SW_SHOW);
    if(::IsIconic(hWnd) || ::GetForegroundWindow() != hWnd)
        ::SwitchToThisWindow(hWnd, TRUE);

    LPCTSTR filePath = reinterpret_cast<LPCTSTR>(cds->lpData);
    if(filePath != NULL && filePath[0] != 0 && File::IsFileExists(filePath))
        OpenImage(filePath);
}

void ImgViewer::OnDropFiles(HDROP hDrop)
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
}

void ImgViewer::OnMouseWheel(short delta)
{
    if(image_ == NULL)
        return;

    int rate = delta / WHEEL_DELTA;
    if(rate > 0 && rate_ > 10)
        return;
    if(rate < 0 && rate_ < -10)
        return;
    rate_ += rate;
    if(rate_ == 0)
        return;

    NSize size = image_->GetPreferSize();
    GetProperSize(size);
    window_->SetSize(size.Width, size.Height);
    window_->Invalidate();
}

void ImgViewer::InitControls()
{
    /*
    NButton* pBtnLeft = NULL;
    NButton* pBtnRight = NULL;
    NReflectCreate(pBtnLeft);
    NReflectCreate(pBtnRight);
    */
}
