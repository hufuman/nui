#include "stdafx.h"
#include "NCoreImpl.h"

#include "../NMemTool.h"
#include "../../data/NStringBundle.h"
#include "../../data/NFileSystem.h"
#include "../../util/NFileUtil.h"
#include "../../ui/implement//NUiProcImpl.h"
#include "DelayedReleaser.h"

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

bool NCoreImpl::InitCore(LPCTSTR packFilePath, LPCTSTR lang, nui::Ui::NRenderType::Type type)
{
    Gdiplus::GdiplusStartupInput input;
    Gdiplus::Status status = Gdiplus::GdiplusStartup(&m_uGdiplusToken, &input, NULL);
    NAssertError(status == Gdiplus::Ok, _T("Failed to init Gdiplus"));
    if(status != Gdiplus::Ok)
        return false;

    m_hOleInitResult = ::OleInitialize(NULL);
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

    nui::Base::NInstPtr<nui::Data::NFileSystem> fileSystem(MemToolParam);
    NAssertError(fileSystem != NULL, _T("Failed to create NFileSystem"));
    if(!fileSystem)
        return false;

    bool initResult = true;
    bool nonEmptyPackFile = (nui::Util::File::IsFileExists(packFilePath) || nui::Util::File::IsFolderExists(packFilePath));
    if(nonEmptyPackFile)
        initResult = fileSystem->Init(packFilePath);
    NAssertError(initResult, _T("Failed to init NFileSystem"));
    if(!initResult)
        return false;

    nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
    NAssertError(stringBundle != NULL, _T("Failed to create NStringBundle"));
    if(!stringBundle)
        return false;

    if(nonEmptyPackFile)
    {
        initResult = stringBundle->InitStringBundle(lang);
        NAssertError(initResult, _T("Failed to init NStringBundle"));
        if(!initResult)
            return false;
    }

    nui::Ui::NUiBus::Instance().Init(type);

    nui::Ui::Impl::NUiProc::Get().Init();

    return true;
}

void NCoreImpl::DestroyCore()
{
    DelayedReleaser::GetInstance().ReleaseAll();

    nui::Ui::NUiBus::Instance().Destroy();
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

    nui::Ui::Impl::NUiProc::Get().Destroy();

    nui::Base::RemoveMemLog(nui::Base::MemTypeNew, this);
    nui::Base::CheckMemLeak();
    nui::Base::ReleaseMemChecker();
}
