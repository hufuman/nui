#pragma once



#include "HttpUtil.h"

enum WeChatMsgType
{
    WeChatMsgUnknown,
    WeChatMsgText
};

class WeChatMsg
{
public:
    NString FromUserName;
    NString ToUserName;
    NString Content;
    WeChatMsgType MsgType;
    NString MsgId;
};
typedef std::list<WeChatMsg*> WeChatMsgList;

class UserInfo
{
public:
    UserInfo()
    {
        data = 0;
        sex_ = 0;
        contactFlag_ = 0;
    }

    UserInfo(UserInfo& right)
    {
        CopyData(right);
    }

    UserInfo& operator = (UserInfo& right)
    {
        CopyData(right);
        return *this;
    }

    ~UserInfo()
    {
        WeChatMsgList::iterator ite = msgList_.begin();
        for(; ite != msgList_.end(); ++ ite)
        {
            WeChatMsg* msg = *ite;
            delete msg;
        }
        msgList_.clear();
    }

    int sex_;
    int contactFlag_;
    NString userName;
    NString nickName;
    NString displayName;
    NString remarkName;
    NString headImgUrl;
    NString signature_;
    NString searchPinyins_;
    DWORD data;

    const NString& GetName() const
    {
        if(!displayName.IsEmpty())
            return displayName;
        if(!remarkName.IsEmpty())
            return remarkName;
        if(!nickName.IsEmpty())
            return nickName;
        return userName;
    }

    void AddMsg(WeChatMsg* msg)
    {
        msgList_.push_back(msg);
    }

    const WeChatMsgList& GetMsgList() const
    {
        return msgList_;
    }

    bool IsMatch(const NString& text) const
    {
        return userName.ToLower().IndexOf(text) >= 0
            || nickName.ToLower().IndexOf(text) >= 0
            || displayName.ToLower().IndexOf(text) >= 0
            || remarkName.ToLower().IndexOf(text) >= 0
            || searchPinyins_.ToLower().IndexOf(text) >= 0;
    }

private:
    WeChatMsgList msgList_;
    void CopyData(UserInfo& right)
    {
        if(this == &right)
            return;
        userName = right.userName;
        nickName = right.nickName;
        displayName = right.displayName;
        remarkName = right.remarkName;
        headImgUrl = right.headImgUrl;
        data = right.data;
        searchPinyins_ = right.searchPinyins_;
        signature_ = right.signature_;
        contactFlag_ = right.contactFlag_;
        sex_ = right.sex_;
        msgList_ = right.msgList_;
        right.msgList_.clear();
    }
};

typedef fastdelegate::FastDelegate2<Base::NBaseObj*, NEventData*, bool> NEventHandler;

class WeChatLogic
{
    WeChatLogic();
    WeChatLogic(const WeChatLogic&);
    WeChatLogic& operator = (const WeChatLogic&);
public:
    ~WeChatLogic(void);

    static WeChatLogic& Get();

public:
    bool DownloadUuid();
    NString DownloadQrCode();

    // 0 - failed
    // 1 - should wait
    // 2 - success
    int CheckScan();

    // 0 - failed
    // 1 - should wait
    // 2 - success
    int CheckLogin();
    bool FetchUserAuth();
    bool FetchUserInfo();

    bool FetchContracts();

    // 
    WeChatMsg* SendTextMsg(LPCTSTR toUserName, LPCTSTR content);

    /**
     *
     *
     * @retcode 1100 - wechat exited
     *          1101 - logon web wechat on other place
     *          0 - success
     *
     * @selector retcode == 0 && selector == 2, have messages
     *
     *
     */
    bool QueryMsgExists(int& retcode, int& selector);
    bool LoadMsgContent(WeChatMsgList& msgs);

    typedef std::list<UserInfo*> UserInfoList;
    UserInfoList& GetUserInfoList();

    const UserInfo& GetSelfInfo() const;

    UserInfo* GetUserInfo(NString userName) const;

private:
    bool ParseUserInfo(const Json::Value& user, UserInfo* userInfo);
    void ResetSyncKey(const Json::Value& value);
    void SetBaseRequest(Json::Value& value);
    void SetMsgContent(Json::Value& value, LPCTSTR toUserName, LPCTSTR content);

private:
    NString uuid_;
    NString redirectUrl_;
    NString baseUrl_;

    NString uin_;
    NString ticket_;
    NString loadTime_;
    NString uvid_;
    NString lang_;
    NString sid_;
    NString cookie_;

    UserInfo self_;

    NString sKey_;
    NString flatSyncKey_;
    NString syncKey_;

    // UserName => UserInfo
    typedef std::map<NString, UserInfo*> UserInfoMap;

    UserInfoList userInfoList_;
    UserInfoMap userInfoMap_;

};
