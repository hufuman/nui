// UIMocker.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "Mocker.h"

void showHelp(LPCTSTR msg);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    NString resourcePath;
    NString styleName;
    for(int i=1; i<__argc; ++ i)
    {
        if(_tcsicmp(__targv[i-1], _T("/res")) == 0)
        {
            resourcePath = __targv[i];
        }
        else if(_tcsicmp(__targv[i-1], _T("/style")) == 0)
        {
            styleName = __targv[i];
        }
    }

    if(styleName.IsEmpty())
    {
        showHelp(_T("no style found"));
        return 0;
    }

    if(resourcePath.IsEmpty())
    {
        resourcePath = NModule::GetInst().GetAppPath();
    }

    if(!File::IsFileExists(File::CombinePath(resourcePath, _T("package.xml"))))
    {
        showHelp(_T("bad resource path"));
        return 0;
    }

    nui::Base::NInstPtr<nui::Base::NCore> core(MemToolParam);
    if(!core->InitCore(resourcePath.GetData(), _T("2052"), NRenderType::GdiRender))
    {
        showHelp(_T("failed to init ui core"));
        return 0;
    }

    {
        NString error;
        CMocker mocker;
        if(!mocker.mock(styleName, error))
        {
            showHelp(error.GetData());
        }
    }

    core->DestroyCore();
	return 0;
}

void showHelp(LPCTSTR msg)
{
    NString message;
    message.Format(_T("Usage:\r\n\r\n  UIMocker [/res <resource base path>] /style @Main:MainUI\r\n\r\n"));
    if(msg && msg[0] != 0)
    {
        message += _T("\r\nError: \r\n\r\n  ");
        message += msg;
    }
    ::MessageBox(NULL, message.GetData(), _T("UIMocker"), MB_OK | MB_ICONERROR);
}
