#include "StdAfx.h"
#include "ResultUi.h"

ResultUi::ResultUi(void) : window_(MemToolParam)
{
}

ResultUi::~ResultUi(void)
{
}

void ResultUi::Show(NWindow* window, const NString& result)
{
    result_ = result;

    window_->SetStyle(WindowStyle::Top);
    window_->WindowCreatedEvent.AddHandler(this, &ResultUi::OnWindowCreated);
    window_->SetVisible(true);
    window_->DoModalWithStyle(window->GetNative(), _T("@ResultUi:MainUi"));

    window_ = NULL;
}

bool ResultUi::OnWindowCreated(Base::NBaseObj* source, NEventData* eventData)
{
    NFrame* rootFrame = window_->GetRootFrame();
    rootFrame->GetChildById<NFrame*>(_T("resultLabel"))->SetText(result_);
    rootFrame->GetChildById<NFrame*>(_T("btnExport"))->ClickEvent.AddHandler(this, &ResultUi::OnBtnExport);

    return true;
}

bool ResultUi::OnBtnExport(Base::NBaseObj* source, NEventData* eventData)
{
    NString filePath;
    if(!nui::Util::Shell::BrowseForFile(filePath, window_->GetNative(), false, NULL))
        return true;
    HANDLE fileHandle = ::CreateFile(filePath, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, 0, 0);
    if(fileHandle == INVALID_HANDLE_VALUE)
    {
        ::MessageBox(window_->GetNative(), _T("Failed to open file"), _T("Error"), MB_OK | MB_ICONERROR);
        return true;
    }
    DWORD dwWritten = 0;
    BYTE bom[] = {0xFF, 0xFE};
    ::WriteFile(fileHandle, bom, 2, &dwWritten, NULL);
    ::WriteFile(fileHandle, result_.GetData(), result_.GetLength() * sizeof(TCHAR), &dwWritten, NULL);
    ::CloseHandle(fileHandle);
    return true;
}
