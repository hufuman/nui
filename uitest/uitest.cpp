// uitest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "uitest.h"

nui::Base::NString GetResourcePath();
bool MsgFilterCallbackProc(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
void InitWindow(NWindow* window);

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
    nui::Base::NInstPtr<nui::Base::NCore> core(MemToolParam);
    core->InitCore(resPath.GetData(), _T("2052"), NRenderType::GdiRender);

    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();

    g_Render = NUiBus::Instance().CreateRender();

    g_Image = loader->LoadImage(_T("@skin:images\\3.gif"));

    g_SingleLineText = loader->CreateText(_T("single line"), MemToolParam);
    g_SingleLineText->SetSingleLine(true).SetColor(MakeArgb(255, 0, 0, 0)).SetHorzCenter(true).SetVertCenter(true);

    g_MultipleLineText = loader->CreateText(_T("line\r\nfirst line\r\nsecond line\r\nthird line"), MemToolParam);
    g_MultipleLineText->SetSingleLine(false).SetColor(MakeArgb(255, 0, 255, 0)).SetHorzCenter(true).SetVertCenter(true);

    NInstPtr<NWindow> window(MemToolParam);
    window->SetMsgFilterCallback(&MsgFilterCallbackProc);
    window->Create(NULL);
    window->SetSize(500, 400);
    window->CenterWindow(NULL);
    window->SetText(_T("Test Window"));
    window->SetVisible(true);

    InitWindow(window);

    {
        nui::Base::NHolder timer;
        NInstPtr<nui::Ui::NTimerSrv> timerSrv(MemToolParam);
        // timer = timerSrv->startTimer(200, MakeDelegate(&window, &NWindow::Invalidate));

        nui::Ui::NMsgLoop loop;
        loop.Loop(window->GetNative());

        g_Image = NULL;
        g_SingleLineText = NULL;
        g_MultipleLineText = NULL;
        g_Render = NULL;
    }

    window = NULL;

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

bool MsgFilterCallbackProc(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
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

void InitWindow(NWindow* window)
{
    NRichFrame* rootFrame = window->GetRootFrame();
    NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
    rootFrame->SetBkgDraw(loader->CreateShape(MemToolParam)->SetFillColor(MakeArgb(200, 255, 255, 0)));

    NRect rect;
    window->GetRect(rect);
    rect.Offset(-rect.Left, -rect.Top);

    Base::NInstPtr<NRichFrame> leftTopFrame(MemToolParam);
    leftTopFrame->SetPos(0, 0);
    leftTopFrame->SetId(_T("leftTop"));
    leftTopFrame->SetSize(rect.Width() / 2, rect.Height() / 2);
    leftTopFrame->SetBkgDraw(loader->CreateShape(MemToolParam)->SetFillColor(MakeArgb(200, 0, 0, 255)));

    Base::NInstPtr<NRichFrame> rightTopFrame(MemToolParam);
    rightTopFrame->SetPos(rect.Width() / 2, 0);
    rightTopFrame->SetId(_T("rightTop"));
    rightTopFrame->SetSize(rect.Width() / 2, rect.Height() / 2);
    rightTopFrame->SetBkgDraw(loader->LoadImage(_T("@skin:images\\3.gif")));

    Base::NInstPtr<NRichFrame> rightBottomFrame(MemToolParam);
    rightBottomFrame->SetPos(rect.Width() / 2, rect.Height() / 2);
    rightBottomFrame->SetId(_T("rightBottom"));
    rightBottomFrame->SetSize(rect.Width() / 2, rect.Height() / 2);
    rightBottomFrame->SetBkgDraw(loader->CreateShape(MemToolParam)->SetFillColor(MakeArgb(200, 0, 255, 0)));

    rootFrame->AddChild(leftTopFrame);
    rootFrame->AddChild(rightTopFrame);
    rootFrame->AddChild(rightBottomFrame);
}

