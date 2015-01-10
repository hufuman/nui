#include "stdafx.h"
#include "NCoreImpl.h"

#include "../NMemTool.h"
#include "../../data/NFileSystem.h"

IMPLEMENT_REFLECTION_EX(NCoreImpl, nui::Base::NReflect::Singleton);


NCoreImpl::NCoreImpl(void)
{
    m_uGdiplusToken = 0;
    m_hOleInitResult = E_NOTIMPL;
}

NCoreImpl::~NCoreImpl(void)
{
    ;
}

bool NCoreImpl::InitCore(LPCTSTR packFilePath, LPCTSTR lang)
{
    UNREFERENCED_PARAMETER(packFilePath);
    UNREFERENCED_PARAMETER(lang);

    Gdiplus::GdiplusStartupInput input;
    Gdiplus::Status status = Gdiplus::GdiplusStartup(&m_uGdiplusToken, &input, NULL);
    NAssertError(status == Gdiplus::Ok, _T("Failed to init Gdiplus"));
    if(status != Gdiplus::Ok)
        return false;

    m_hOleInitResult = OleInitialize(NULL);
    NAssertError(SUCCEEDED(m_hOleInitResult), _T("Failed to init Ole"));
    if(FAILED(m_hOleInitResult))
        return false;

    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwSize = sizeof(InitCtrls);
    InitCtrls.dwICC = ICC_WIN95_CLASSES;
    if(!InitCommonControlsEx(&InitCtrls))
    {
        NAssertError(false, _T("Failed to init common controls"));
        return false;
    }

    nui::Base::NInstPtr<nui::Data::NFileSystem> fileSystem(InstPtrParam);
    if(!fileSystem)
        return false;

    return fileSystem->Init(packFilePath);
}

void NCoreImpl::DestroyCore()
{
    if(m_uGdiplusToken != 0)
    {
        Gdiplus::GdiplusShutdown(m_uGdiplusToken);
        m_uGdiplusToken = 0;
    }
    if(SUCCEEDED(m_hOleInitResult))
    {
        OleUninitialize();
        m_hOleInitResult = E_NOTIMPL;
    }

    nui::Base::NReflect::GetInstance().ReleaseData(this);
    nui::Base::RemoveMemLog(nui::Base::MemTypeNew, this);
    nui::Base::CheckMemLeak();
    nui::Base::ReleaseMemChecker();
}
