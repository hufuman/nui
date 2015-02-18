// uitest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "uitest.h"
#include "NWnd.h"

nui::Base::NString GetResourcePath();
bool PaintCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
NAutoPtr<NImage> g_Image;
NAutoPtr<NText> g_SingleLineText;
NAutoPtr<NText> g_MultipleLineText;

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

    g_Image = loader->LoadImage(_T("@skin:images\\3.gif"));

    g_SingleLineText = loader->CreateText(_T("single line"));
    g_SingleLineText->SetSingleLine(true).SetColor(MakeArgb(255, 0, 0, 0)).SetHorzCenter(true).SetVertCenter(true);

    g_MultipleLineText = loader->CreateText(_T("line\r\nfirst line\r\nsecond line\r\nthird line"));
    g_MultipleLineText->SetSingleLine(false).SetColor(MakeArgb(255, 0, 255, 0)).SetHorzCenter(true).SetVertCenter(true);

    NWnd window;
    window.SetMsgFilterCallback(PaintCallback);
    window.Create(NULL);
    window.SetSize(500, 400);
    window.CenterWindow(NULL);
    window.SetText(_T("Test Window"));
    window.SetVisible(true);

    {
        nui::Base::NHolder timer;
        NInstPtr<nui::Ui::NTimerSrv> timerSrv(InstPtrParam);
        timer = timerSrv->startTimer(200, MakeDelegate(&window, &NWnd::Invalidate));

        nui::Ui::NMsgLoop loop;
        loop.Loop(window.GetNative());

        g_Image = NULL;
        g_SingleLineText = NULL;
        g_MultipleLineText = NULL;
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

bool PaintCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    if(message != WM_PAINT)
        return false;

    NRect rect;
    ::GetClientRect(hWnd, reinterpret_cast<RECT*>(&rect));

    PAINTSTRUCT ps = {0};
    HDC hDc = ::BeginPaint(hWnd, &ps);

    nui::Base::NAutoPtr<nui::Ui::NRender> render = NUiBus::Instance().GetRender();
    render->Init(hDc, rect);
    render->DrawRectangle(rect, nui::Ui::MakeArgb(255, 0, 255, 255));

    g_Image->NextFrame();
    render->DrawImage(g_Image, 0, 0);
    NSize size = g_Image->GetSize();
    render->DrawImage(g_Image, rect.Width() - size.Width, rect.Height() - size.Height);

    NRect textRect(rect);

    g_SingleLineText->SetColor(MakeArgb(255, 255, 0, 255));
    render->DrawText(g_SingleLineText, textRect);

    g_SingleLineText->SetColor(MakeArgb(255, 0, 0, 255));
    textRect.Top += 40;
    render->DrawText(g_SingleLineText, textRect);

    textRect.Top -= 40;
    g_MultipleLineText->SetVertCenter(true).SetHorzCenter(true);
    render->DrawText(g_MultipleLineText, textRect);
    g_MultipleLineText->SetVertCenter(false).SetHorzCenter(false);
    render->DrawText(g_MultipleLineText, textRect);

    render->DrawBack();

    ::EndPaint(hWnd, &ps);

    return true;
}

/*
// demonstrate DrawShape
bool PaintCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult)
{
    if(message != WM_PAINT)
        return false;

    NRect rect;
    ::GetClientRect(hWnd, reinterpret_cast<RECT*>(&rect));

    PAINTSTRUCT ps = {0};
    HDC hDc = ::BeginPaint(hWnd, &ps);

    nui::Base::NAutoPtr<nui::Ui::NRender> render = NUiBus::Instance().GetRender();
    render->Init(hDc, rect);

    render->DrawRectangle(rect, nui::Ui::MakeArgb(255, 255, 255, 255));

    for(int i=0; i<3; ++ i)
    {
        for(int j=0; j<3; ++ j)
        {
            NRect rc(rect);
            rc.Left = rect.Width() * i / 3;
            rc.Right = rc.Left + rect.Width() / 3;
            rc.Top = rect.Height() * j / 3;
            rc.Bottom = rc.Top + rect.Height() / 3;

            int index = i * 3 + j;
            BYTE red = (index & 0x04) == 0x04 ? 255 : 0;
            BYTE green = (index & 0x02) == 0x02 ? 255 : 0;
            BYTE blue = (index & 0x01) == 0x01 ? 255 : 0;

            for(int m=0; m<3; ++ m)
            {
                for(int n=0; n<3; ++ n)
                {
                    NRect r(rc);
                    r.Left = rc.Left + rc.Width() * m / 3;
                    r.Right = r.Left + rc.Width() / 3;
                    r.Top = rc.Top + rc.Height() * n / 3;
                    r.Bottom = r.Top + rc.Height() / 3;

                    BYTE alpha = (m * 3 + n) * 255 / 9;
                    render->DrawRectangle(r, nui::Ui::MakeArgb(alpha, red, green, blue));
                }
            }
        }
    }

    render->DrawRectangle(rect, 4, nui::Ui::MakeArgb(255, 255, 255, 0));

    render->DrawBack();

    ::EndPaint(hWnd, &ps);

    return true;
}
*/