#include "StdAfx.h"
#include "MainUi.h"

#include "WeChatLogic.h"


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
    NLayout* layout = window_->GetRootFrame()->GetChildById<NLayout*>(_T("contactList"));
    const WeChatLogic::UserInfoList& userInfoList = WeChatLogic::Get().GetUserInfoList();
    WeChatLogic::UserInfoList::const_iterator ite = userInfoList.begin();

    NInstPtr<NParser> parser(MemToolParam);
    for(; ite != userInfoList.end(); ++ ite)
    {
        const UserInfo* const & user = *ite;
        NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)parser->Parse(layout, _T("@MainUi:RbContact")));
    }
    return false;
}
