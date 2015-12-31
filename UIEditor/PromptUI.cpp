#include "StdAfx.h"
#include "PromptUI.h"

CPromptUI::CPromptUI(void)
{
    window_ = NULL;
    rootFrame_ = NULL;
}

CPromptUI::~CPromptUI(void)
{
}

NString CPromptUI::DoModal(HWND hWnd)
{
    NInstPtr<NWindow> window(MemToolParam);

    window_ = window;
    window->WindowCreatedEvent.AddHandler(this, &CPromptUI::onWindowCreated);
    window->DoModalWithStyle(hWnd, _T("@PromptUI:MainUI"));
    return styleName_;
}

bool CPromptUI::onWindowCreated(NBaseObj*, NEventData*)
{
    rootFrame_ = window_->GetRootFrame();
    rootFrame_->GetChildById(_T("btnPreview"))->ClickEvent += MakeDelegate(this, &CPromptUI::onBtnPreview);

    return true;
}

bool CPromptUI::onBtnPreview(NBaseObj*, NEventData*)
{
    NEdit* editStyle = rootFrame_->GetChildById<NEdit*>(_T("editStyle"));
    styleName_ = editStyle->GetText();
    window_->Destroy();
    return true;
}