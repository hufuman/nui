// uitest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "uitest.h"

NString GetResourcePath();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    NString resPath = GetResourcePath();
    NInstPtr<NCore> core(InstPtrParam);
    core->InitCore(resPath.GetData(), _T("2052"));

    nui::UI::NWindowBase window;
    NRect rcWnd;
    window.Create(NULL, _T("Test"), rcWnd);
    window.SetVisible(TRUE);

    NMsgLoop loop;
    loop.Loop(window.GetNative());
	return 0;
}

NString GetResourcePath()
{
    NString resPath = NModule::GetInst().GetAppPath();
    NString tmp = File::CombinePath(resPath, _T("uidata"));
    if(!File::IsFileExists(tmp.GetData()))
        tmp = File::CombinePath(File::GetParentFolder(resPath), _T("uidata"));
    if(!File::IsFileExists(tmp.GetData()))
        tmp = File::CombinePath(File::GetParentFolder(resPath), _T("uidata"));
    return tmp;
}
