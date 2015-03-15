// imgviewer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "imgviewer.h"

nui::Base::NString GetResourcePath();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    nui::Base::NInstPtr<nui::Base::NCore> core(MemToolParam);
    core->InitCore(_T(""), _T("2052"), NRenderType::GdiRender);

    ImgViewer viewer;
    viewer.Show();

    nui::Ui::NMsgLoop loop;
    loop.Loop(viewer.GetNative());

    viewer.Destroy();

    core->DestroyCore();
	return 0;
}

