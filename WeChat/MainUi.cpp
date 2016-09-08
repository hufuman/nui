#include "StdAfx.h"
#include "MainUi.h"

#include "config.h"
#include "WeChatLogic.h"
#include "ImageLoader.h"
#include "UiUtil.h"

MainUi::MainUi(void) : window_(MemToolParam)
{
    needShowLogin_ = false;
    currentUser_ = NULL;
    msgContainer_ = NULL;
    msgLayout_ = NULL;
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

    emotionUi_.Destroy();

    HttpUtil::StopAllRequest();
    loadMsgThread_.StopAndWait();
    CImageLoader::Get().Stop();

    return needShowLogin_;
}

bool MainUi::OnWindowCreated(Base::NBaseObj* source, NEventData* eventData)
{
    NFrame* rootFrame = window_->GetRootFrame();
    NLayout* layout = rootFrame->GetChildById<NLayout*>(_T("contactList"));
    msgContainer_ = rootFrame->GetChildById<NFrame*>(_T("msgContainer"));
    WeChatLogic::UserInfoList& userInfoList = WeChatLogic::Get().GetUserInfoList();
    WeChatLogic::UserInfoList::iterator ite = userInfoList.begin();

    editContent_ = rootFrame->GetChildById<NEdit*>(_T("MsgContent"));
    rootFrame->GetChildById<NFrame*>(_T("sendMsg"))->ClickEvent.AddHandler(this, &MainUi::OnBtnSend);
    rootFrame->GetChildById<NFrame*>(_T("btnEmotion"))->ClickEvent.AddHandler(this, &MainUi::OnBtnEmotionClicked);
    rootFrame->GetChildById<NEdit*>(_T("editFilter"))->TextChangedEvent.AddHandler(this, &MainUi::OnEditFilter);
    rootFrame->GetChildById<NFrame*>(_T("btnCancelFilter"))->ClickEvent.AddHandler(this, &MainUi::OnBtnCancelFilter);

    NInstPtr<NParser> parser(MemToolParam);
    for(; ite != userInfoList.end(); ++ ite)
    {
        UserInfo* & user = *ite;
        NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)parser->Parse(layout, _T("@MainUi:RbContact")));

        UiUtil::SetFrameText(frame, _T("title"), user->GetName())
            ->SetData(reinterpret_cast<DWORD>(user))
            ->ClickEvent.AddHandler(this, &MainUi::OnContactClicked);

        frame->GetChildById(_T("avatar"))
            ->SetData(reinterpret_cast<DWORD>(user))
            ->PreDrawEvent.AddHandler(this, &MainUi::OnAvatarPreDraw);

        user->data = reinterpret_cast<DWORD>((NFrame*)frame);
    }

    emotionUi_.Create(window_);
    emotionUi_.EmotionSelectedEvent.AddHandler(fastdelegate::MakeDelegate(this, &MainUi::OnEmotionSelected));
    loadMsgThread_.Start(MakeDelegate(this, &MainUi::LoadMsgThreadProc));
    return false;
}

bool MainUi::OnContactClicked(Base::NBaseObj* source, NEventData* eventData)
{
    NAutoPtr<NFrame> contactFrame = dynamic_cast<NFrame*>(source);
    UserInfo* user = reinterpret_cast<UserInfo*>(contactFrame->GetData());
    ShowContact(user, contactFrame);
    return false;
}

bool MainUi::OnBtnSend(Base::NBaseObj* source, NEventData* eventData)
{
    if(currentUser_ == NULL)
        return false;
    NString text = editContent_->GetText();
    if(text.IsEmpty())
        return false;
    WeChatMsg* msg = WeChatLogic::Get().SendTextMsg(currentUser_->userName, text);
    editContent_->SetText(_T(""));
    WeChatMsgList listMsgs;
    listMsgs.push_back(msg);
    OnMsgArrived(&listMsgs);
    return false;
}

bool MainUi::OnEditFilter(Base::NBaseObj* source, NEventData* eventData)
{
    if(filterTimer_)
        return false;
    NInstPtr<NTimerSrv> timer(MemToolParam);
    filterTimer_ = timer->startOnceTimer(200, MakeDelegate(this, &MainUi::DoFilter));
    return false;
}

bool MainUi::OnBtnCancelFilter(Base::NBaseObj* source, NEventData* eventData)
{
    UiUtil::SetFrameText(window_->GetRootFrame(), _T("editFilter"), _T(""));
    DoFilter();
    return false;
}

bool MainUi::OnAvatarPreDraw(Base::NBaseObj* source, NEventData* eventData)
{
    NImage* avatar = dynamic_cast<NImage*>(source);
    if(avatar->GetData() != 1)
    {
        avatar->PreDrawEvent.RemoveHandler(MakeDelegate(this, &MainUi::OnAvatarPreDraw));
        UserInfo* user = reinterpret_cast<UserInfo*>(avatar->GetData());
        avatar->SetData(1);
        NString imgPath = CImageLoader::Get().LoadImage(avatar, user->headImgUrl, true);
        user->headImgPath = imgPath;
    }
    return false;
}

bool MainUi::OnBtnEmotionClicked(Base::NBaseObj* source, NEventData* eventData)
{
    NFrame* frame = dynamic_cast<NFrame*>(source);
    emotionUi_.Show(frame->GetParent());
    return false;
}

bool MainUi::OnEmotionSelected(Base::NBaseObj* source, NEventData* eventData)
{
    EmotionUi::EmotionEventData* data = static_cast<EmotionUi::EmotionEventData*>(eventData);
    DWORD dwLength = ::GetWindowTextLength(editContent_->GetNative());
    NString text;
    text.Format(_T("%[%s%]"), (LPCTSTR)data->emotion);
    editContent_->PasteText(text);
    return false;
}

void MainUi::AddMsgs(const WeChatMsgList& listMsgs, bool needRelayout)
{
    WeChatMsg* lastMsg = NULL;
    NInstPtr<NParser> parser(MemToolParam);
    const UserInfo& selfInfo = WeChatLogic::Get().GetSelfInfo();
    WeChatMsgList::const_iterator ite = listMsgs.begin();
    NLayout* contactList = window_->GetRootFrame()->GetChildById<NLayout*>(_T("contactList"));
    if(needRelayout)
        contactList->SetLayoutable(false);
    if(msgLayout_ != NULL)
        msgLayout_->SetLayoutable(false);
    for(; ite != listMsgs.end(); ++ ite)
    {
        WeChatMsg* msg = *ite;

        if(msg->MsgType != WeChatMsgPlain)
            continue;

        UserInfo* fromUserInfo = WeChatLogic::Get().GetUserInfo(msg->FromUserName);
        UserInfo* targetUserInfo = fromUserInfo->userName == selfInfo.userName ? WeChatLogic::Get().GetUserInfo(msg->ToUserName) : fromUserInfo;

        if(targetUserInfo != NULL)
        {
            NFrame* frame = reinterpret_cast<NFrame*>(targetUserInfo->data);

            if(needRelayout)
                contactList->GetChildById<NFrame*>(_NUI_INNER_FRAME_ID_)->SetChildZOrder(frame, 0);
            frame->GetChildById(_T("msg"))->SetText(msg->Content);
        }

        if(fromUserInfo == NULL)
            continue;

        if(currentUser_ == NULL)
            continue;

        if(msg->FromUserName != currentUser_->userName && msg->ToUserName != currentUser_->userName)
            continue;

        if(msgLayout_ == NULL)
            continue;

        bool isSelf = fromUserInfo->userName == selfInfo.userName;
        NString styleName = isSelf ? _T("@MainUi:SelfMsg") : _T("@MainUi:OtherMsg");
        NFrame* msgFrame = dynamic_cast<NFrame*>((NBaseObj*)parser->Parse(msgLayout_, styleName));
        NImage* avatar = msgFrame->GetChildById<NImage*>(_T("avatar"));
        msgFrame->GetChildById<NLabel*>(_T("msg"))->SetText(msg->Content);
        if(fromUserInfo->headImgPath.IsEmpty())
            fromUserInfo->headImgPath = CImageLoader::Get().LoadImage(avatar, fromUserInfo->headImgUrl, true);
        else
            avatar->LoadImage(fromUserInfo->headImgPath);
    }
    if(msgLayout_ != NULL)
        msgLayout_->SetLayoutable(true);
    if(needRelayout)
        contactList->SetLayoutable(true);
}

void MainUi::ShowContact(UserInfo* user, NFrame* frame)
{
    if(currentUser_ == user)
        return;

    currentUser_ = user;
    if(msgLayout_ != NULL)
    {
        msgContainer_->RemoveChild(msgLayout_);
        msgLayout_ = NULL;
    }
    NInstPtr<NParser> parser(MemToolParam);
    msgLayout_ = dynamic_cast<NLayout*>((NBaseObj*)parser->Parse(msgContainer_, _T("@MainUi:msgLayout")));

    const WeChatMsgList& msgList = currentUser_->GetMsgList();
    if(msgList.empty())
        return;
    AddMsgs(msgList, false);
}

void MainUi::LoadMsgThreadProc(bool& stopping)
{
    int retCode = 0;
    int selector = 0;
    WeChatMsgList msgs;
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
            Sleep(500);
            continue;
        }

        if(!WeChatLogic::Get().LoadMsgContent(msgs))
        {
            Sleep(500);
            continue;
        }

        int result = SendUiProc(MakeDelegate(this, &MainUi::OnMsgArrived), reinterpret_cast<void*>(&msgs));
    }
    if(needShowLogin_ && window_ != NULL)
    {
        window_->Destroy();
    }
}

LRESULT MainUi::OnMsgArrived(void* param)
{
    WeChatMsgList* listMsgs = reinterpret_cast<WeChatMsgList*>(param);
    AddMsgs(*listMsgs, true);
    return 0;
}

void MainUi::DoFilter()
{
    NFrame* rootFrame = window_->GetRootFrame();
    NEdit* editFilter = rootFrame->GetChildById<NEdit*>(_T("editFilter"));
    NLayout* contactList = rootFrame->GetChildById<NLayout*>(_T("contactList"));

    NString text = editFilter->GetText();

    NFrame* btnCancelFilter = rootFrame->GetChildById<NFrame*>(_T("btnCancelFilter"));
    btnCancelFilter->SetVisible(!text.IsEmpty());

    contactList->SetLayoutable(false);
    contactList->GetChildById<NFrame*>(_NUI_INNER_FRAME_ID_)->EnumChilds(MakeDelegate(this, &MainUi::DoFilterImpl), reinterpret_cast<LPARAM>(&text));
    contactList->SetLayoutable(true);
    filterTimer_.Release();
}

bool MainUi::DoFilterImpl(NFrameBase* child, LPARAM lParam)
{
    NString* text = reinterpret_cast<NString*>(lParam);
    bool visible = text->IsEmpty();
    if(!visible)
    {
        UserInfo* user = reinterpret_cast<UserInfo*>(child->GetData());
        if(user->IsMatch(*text))
            visible = true;
    }
    child->SetVisible(visible);
    return true;
}
