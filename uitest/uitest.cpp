// uitest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "uitest.h"

#include "ControlTest.h"
#include "TestDrawImage.h"

nui::Base::NString GetResourcePath();

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

    /*
    CTestDrawImage testDrawImage;
    testDrawImage.Test();//*/

    CControlTest controlTest;
    controlTest.Test();

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
