// uitest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "uitest.h"
#include "NWnd.h"

nui::Base::NString GetResourcePath();
bool PaintCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);
NImage* g_Image;

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

    NWnd window;
    window.SetMsgFilterCallback(PaintCallback);
    window.Create(NULL);
    window.SetSize(500, 400);
    window.CenterWindow(NULL);
    window.SetText(_T("Test Window"));
    window.SetVisible(true);

    nui::Ui::NMsgLoop loop;
    loop.Loop(window.GetNative());

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
