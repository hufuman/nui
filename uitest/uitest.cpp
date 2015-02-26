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

    NWindow* wnd = &window;
    wnd->GetRootFrame()->SetText(_T("Window Text"));
    NRect rect;
    wnd->GetRect(rect);
    int width = rect.Width();
    int height = rect.Height();
    const int count = 2;
    for(int i=0; i<count; ++ i)
    {
        for(int j=0; j<count; ++ j)
        {
            NString msg;
            msg.Format(_T("%d * %d"), i, j);
            NInstPtr<NFrame> frame(InstPtrParam);
            frame->SetText(msg);
            frame->SetPos(i * width / count, j * height / count);
            frame->SetSize(width / count, height / count);
            for(int k=0; k<count; ++ k)
            {
                NString msg;
                msg.Format(_T("%d"), k);
                NInstPtr<NFrame> child(InstPtrParam);
                child->SetText(msg);
                // child->SetPos(width / count / count * k, height / count);
                child->SetSize(width / count / count, height / count);
                frame->AddChild(child);
            }
            wnd->GetRootFrame()->AddChild(frame);
        }
    }

    {
        nui::Base::NHolder timer;
        NInstPtr<nui::Ui::NTimerSrv> timerSrv(InstPtrParam);
        // timer = timerSrv->startTimer(200, MakeDelegate(&window, &NWindow::Invalidate));

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
    }
    return false;
}
