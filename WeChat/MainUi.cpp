#include "StdAfx.h"
#include "MainUi.h"

MainUi::MainUi(void) : window_(MemToolParam)
{
}

MainUi::~MainUi(void)
{
}

void MainUi::Show()
{
    window_->SetStyle(WindowStyle::Top);
    window_->WindowCreatedEvent.AddHandler(this, &MainUi::OnWindowCreated);
    window_->SetVisible(true);
    window_->DoModalWithStyle(NULL, _T("@MainUi:MainUi"));
}

bool MainUi::OnWindowCreated(Base::NBaseObj* source, NEventData* eventData)
{
    return false;
}
