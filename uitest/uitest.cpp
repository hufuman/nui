// uitest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "uitest.h"

nui::Base::NString GetResourcePath();
bool PaintCallback(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
NAutoPtr<NImage> g_Image;
NAutoPtr<NText> g_SingleLineText;
NAutoPtr<NText> g_MultipleLineText;
nui::Base::NAutoPtr<nui::Ui::NRender> g_Render;
int g_BorderWidth = 1;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    nui::Base::NString resPath = GetResourcePath();
    nui::Base::NInstPtr<nui::Base::NCore> core(InstPtrParam);
    core->InitCore(resPath.GetData(), _T("2052"), NRenderType::GdiRender);

    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();

    g_Render = NUiBus::Instance().CreateRender();

    g_Image = loader->LoadImage(_T("@skin:images\\3.gif"));

    g_SingleLineText = loader->CreateText(_T("single line"));
    g_SingleLineText->SetSingleLine(true).SetColor(MakeArgb(255, 0, 0, 0)).SetHorzCenter(true).SetVertCenter(true);

    g_MultipleLineText = loader->CreateText(_T("line\r\nfirst line\r\nsecond line\r\nthird line"));
    g_MultipleLineText->SetSingleLine(false).SetColor(MakeArgb(255, 0, 255, 0)).SetHorzCenter(true).SetVertCenter(true);

    NWindow window;
    window.SetMsgFilterCallback(PaintCallback);
    window.Create(NULL);
    window.SetSize(500, 400);
    window.CenterWindow(NULL);
    window.SetText(_T("Test Window"));
    window.SetVisible(true);

    {
        nui::Base::NHolder timer;
        NInstPtr<nui::Ui::NTimerSrv> timerSrv(InstPtrParam);
        timer = timerSrv->startTimer(200, MakeDelegate(&window, &NWindow::Invalidate));

        nui::Ui::NMsgLoop loop;
        loop.Loop(window.GetNative());

        g_Image = NULL;
        g_SingleLineText = NULL;
        g_MultipleLineText = NULL;
        g_Render = NULL;
    }

    core->DestroyCore();
	return 0;
}

nui::Base::NString GetResourcePath()
{
    nui::Base::NString resPath = nui::Data::NModule::GetInst().GetAppPath();
    nui::Base::NString tmp = nui::Util::File::CombinePath(resPath, _T("uidata"));
    if(!nui::Util::File::IsFolderExists(tmp.GetData()))
    {
        tmp = nui::Util::File::GetParentFolder(tmp);
        tmp = nui::Util::File::GetParentFolder(tmp);
        tmp = nui::Util::File::GetParentFolder(tmp);
        tmp = nui::Util::File::CombinePath(tmp, _T("uidata"));
        tmp = nui::Util::File::CombinePath(tmp, _T("uidata"));
    }
    return tmp;
}

bool PaintCallback(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    HWND hWnd = window->GetNative();
    if(message == WM_KEYDOWN)
    {
        ++ g_BorderWidth;
        ::InvalidateRect(hWnd, NULL, TRUE);
    }
    else if(message == WM_CREATE)
    {
        NWindow* wnd = dynamic_cast<NWindow*>(window);
        wnd->GetRootFrame()->SetText(_T("Window Text"));
    }
    if(message != WM_PAINT)
        return false;

    NRect rect;
    ::GetClientRect(hWnd, reinterpret_cast<RECT*>(&rect));

    PAINTSTRUCT ps = {0};
    HDC hDc = ::BeginPaint(hWnd, &ps);

    g_Render->Init(hDc, rect);

    g_Render->FillRectangle(rect, nui::Ui::MakeArgb(255, 255, 255, 0));

    NRect tmpRect(rect);
    tmpRect.SetRect(0, 0, 310, 310);
    g_Render->DrawRectangle(tmpRect, g_BorderWidth, MakeArgb(255, 255, 0, 0));
    // g_Render->DrawRoundRectangle(tmpRect, 10, MakeArgb(255, 0, 255, 0));
    g_Render->DrawRoundRectangle(tmpRect, g_BorderWidth, MakeArgb(100, 0, 255, 0), MakeArgb(254, 0, 255, 255));

    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    Base::NString msg;
    msg.Format(_T("Border: %d"), g_BorderWidth);
    NAutoPtr<NText> text = loader->CreateText(msg);
    text->SetHorzCenter(true).SetVertCenter(true);
    g_Render->DrawText(text, tmpRect);

    g_Render->DrawBack();

    ::EndPaint(hWnd, &ps);

    return true;
}
