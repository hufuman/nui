#include "StdAfx.h"
#include "ResultUi.h"

ResultUi::ResultUi(void) : window_(MemToolParam)
{
}

ResultUi::~ResultUi(void)
{
}

void ResultUi::Show(const NString& result)
{
    result_ = result;

    window_->SetStyle(WindowStyle::Top);
    window_->WindowCreatedEvent.AddHandler(this, &ResultUi::OnWindowCreated);
    window_->SetVisible(true);
    window_->DoModalWithStyle(NULL, _T("@ResultUi:MainUi"));

    window_ = NULL;
}

bool ResultUi::OnWindowCreated(Base::NBaseObj* source, NEventData* eventData)
{
    NFrame* rootFrame = window_->GetRootFrame();
    rootFrame->GetChildById<NFrame*>(_T("resultLabel"))->SetText(result_);
    return true;
}
