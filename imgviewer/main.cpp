// imgviewer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "imgviewer.h"
#include "FileAssoc.h"
#include "ViewerData.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    ImgViewer viewer;

    if(__argc == 2)
    {
        for(;;)
        {
            BOOL needAssoc = FALSE;
            if(_tcsicmp(__targv[1], _T("/assoc")) == 0)
            {
                needAssoc = TRUE;
            }
            else if(_tcsicmp(__targv[1], _T("/unassoc")) == 0)
            {
                needAssoc = FALSE;
            }
            else
            {
                break;
            }
            const NArrayT<NString>& formats = viewer.GetSupportedFormats();
            FileAssoc fileAssoc;
            fileAssoc.Start();
            int count = formats.Count();
            for(int i=0; i<count; ++ i)
            {
                fileAssoc.Assoc(formats[i]);
            }
            BOOL result = fileAssoc.Roll(needAssoc);
            NString msg;
            msg = needAssoc ? _T("Assoc ") : _T("UnAssoc ");
            msg += result ? _T("Ok") : _T("Failed");
            if(result)
                ::MessageBox(NULL, msg, _T("Image Viewer"), MB_OK | MB_ICONINFORMATION);
            else
                ::MessageBox(NULL, msg, _T("Image Viewer"), MB_OK | MB_ICONERROR);
            return 0;
        }
    }

    nui::Base::NInstPtr<nui::Base::NCore> core(MemToolParam);
    nui::Data::NModule::GetInst().Init(::GetModuleHandle(NULL));
    core->InitCore(_T(""), _T("2052"), NRenderType::GdiRender);

    NString filePath;
    if(__argc == 2 && File::IsFileExists(__targv[1]))
    {
        filePath = __targv[1];
    }
    ViewerData* data = ViewerDataMgr::Instance().Hold(filePath, NULL, FALSE);
    if(data != NULL)
    {
        viewer.ShowPrev(data->hWnd, filePath);
        viewer.Destroy();
        core->DestroyCore();
        return 0;
    }
    viewer.Show(filePath);

    nui::Ui::NMsgLoop loop;
    loop.Loop(viewer.GetNative());

    viewer.Destroy();

    core->DestroyCore();
	return 0;
}

