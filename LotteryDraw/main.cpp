// main.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "MainUi.h"
#include "MiscUtil.h"

#include <ctime>

nui::Base::NString GetResourcePath();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    srand(static_cast<unsigned int>(time(NULL)));
    nui::Data::NModule::GetInst().Init(::GetModuleHandle(NULL));

    nui::Base::NString resPath = GetResourcePath();
    nui::Base::NInstPtr<nui::Base::NCore> core(MemToolParam);
    core->InitCore(resPath.GetData(), _T("2052"), NRenderType::GdiRender);

    MainUi mainUi;
    mainUi.Show();

    core->DestroyCore();

    return 0;
}



nui::Base::NString GetResourcePath()
{
    nui::Base::NString resPath = nui::Data::NModule::GetInst().GetAppPath();
    nui::Base::NString tmp = nui::Util::File::CombinePath(resPath, _T("LotteryDraw"));
    if(!nui::Util::File::IsFolderExists(tmp.GetData()))
    {
        tmp = nui::Util::File::GetParentFolder(tmp);
        tmp = nui::Util::File::GetParentFolder(tmp);
        tmp = nui::Util::File::GetParentFolder(tmp);
        tmp = nui::Util::File::CombinePath(tmp, _T("uidata"));
        tmp = nui::Util::File::CombinePath(tmp, _T("LotteryDraw"));
    }
    return tmp;
}