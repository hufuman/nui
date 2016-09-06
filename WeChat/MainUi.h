#pragma once


#include "WeChatLogic.h"


class MainUi
{
public:
    MainUi(void);
    ~MainUi(void);

    // whether need to show login
    bool Show();

private:
    bool OnWindowCreated(Base::NBaseObj* source, NEventData* eventData);
    bool OnContactClicked(Base::NBaseObj* source, NEventData* eventData);
    bool OnBtnSend(Base::NBaseObj* source, NEventData* eventData);

    UserInfo* AddMsgs(const WeChatMsgList& listMsgs);
    void ShowContact(UserInfo* user, NFrame* frame);
    void LoadMsgThreadProc(bool& stopping);
    LRESULT OnMsgArrived(void*);

private:
    NFrame* msgContainer_;
    NLayout* msgLayout_;
    UserInfo* currentUser_;
    NEdit* editContent_;
    bool needShowLogin_;
    NThread loadMsgThread_;
    NInstPtr<NWindow> window_;
};

