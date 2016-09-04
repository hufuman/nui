#include "StdAfx.h"
#include "MainUi.h"

#include "config.h"
#include "WeChatLogic.h"
#include "ImageLoader.h"

MainUi::MainUi(void) : window_(MemToolParam)
{
    needShowLogin_ = false;
}

MainUi::~MainUi(void)
{
}

bool MainUi::Show()
{
    window_->SetStyle(WindowStyle::Top);
    window_->WindowCreatedEvent.AddHandler(this, &MainUi::OnWindowCreated);
    window_->SetVisible(true);
    window_->DoModalWithStyle(NULL, _T("@MainUi:MainUi"));

    HttpUtil::StopAllRequest();
    loadMsgThread_.StopAndWait();
    CImageLoader::Get().Stop();

    return needShowLogin_;
}

bool MainUi::OnWindowCreated(Base::NBaseObj* source, NEventData* eventData)
{
    NLayout* layout = window_->GetRootFrame()->GetChildById<NLayout*>(_T("contactList"));
    WeChatLogic::UserInfoList& userInfoList = WeChatLogic::Get().GetUserInfoList();
    WeChatLogic::UserInfoList::iterator ite = userInfoList.begin();

    NInstPtr<NParser> parser(MemToolParam);
    for(; ite != userInfoList.end(); ++ ite)
    {
        UserInfo* & user = *ite;
        NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)parser->Parse(layout, _T("@MainUi:RbContact")));
        NLabel* title = frame->GetChildById<NLabel*>(_T("title"));
        NImage* avatar = frame->GetChildById<NImage*>(_T("avatar"));
        title->SetText(user->GetName());
        frame->SetData(reinterpret_cast<DWORD>(user));
        user->data = reinterpret_cast<DWORD>((NFrame*)frame);

        CImageLoader::Get().LoadImage(avatar, user->headImgUrl, true);
    }

    loadMsgThread_.Start(MakeDelegate(this, &MainUi::LoadMsgThreadProc));
    return false;
}

void MainUi::LoadMsgThreadProc(bool& stopping)
{
    int retCode = 0;
    int selector = 0;
    std::list<WeChatMsg> msgs;
    for(;!stopping;)
    {
        if(!WeChatLogic::Get().QueryMsgExists(retCode, selector))
        {
            if(retCode == 1100)
            {
                NInstPtr<NStringBundle> bundle(MemToolParam);
                NString msg = bundle->GetString(_T("@MainUi:errAppExited"));
                NString title = bundle->GetString(_T("@Common:appTitle"));
                if(::MessageBox(window_->GetNative(), msg, title, MB_YESNO | MB_ICONQUESTION) == IDYES)
                    needShowLogin_ = true;
                break;
            }
            else if(retCode == 1101)
            {
                NInstPtr<NStringBundle> bundle(MemToolParam);
                NString msg = bundle->GetString(_T("@MainUi:errLogon"));
                NString title = bundle->GetString(_T("@Common:appTitle"));
                if(::MessageBox(window_->GetNative(), msg, title, MB_YESNO | MB_ICONQUESTION) == IDYES)
                    needShowLogin_ = true;
                break;
            }
            Sleep(1000);
            continue;
        }

        if(!WeChatLogic::Get().LoadMsgContent(msgs))
        {
            Sleep(1000);
            continue;
        }
    }
    if(needShowLogin_ && window_ != NULL)
    {
        window_->Destroy();
    }
}
