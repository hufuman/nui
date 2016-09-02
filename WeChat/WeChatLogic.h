#pragma once



#include "HttpUtil.h"

class UserInfo
{
public:
    UserInfo()
    {
        data = 0;
        sex_ = 0;
        contactFlag_ = 0;
    }

    UserInfo(const UserInfo& right)
    {
        CopyData(right);
    }

    UserInfo& operator = (const UserInfo& right)
    {
        CopyData(right);
        return *this;
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

private:
    void CopyData(const UserInfo& right)
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
    }
};

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

    void Reset()
    {
        FromUserName = _T("");
        ToUserName = _T("");
        Content = _T("");
        MsgType = WeChatMsgUnknown;
        MsgId = _T("");
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
    bool LoadMsgContent(std::list<WeChatMsg>& msgs);

    typedef std::list<UserInfo*> UserInfoList;
    UserInfoList& GetUserInfoList();

private:
    bool ParseUserInfo(const Json::Value& user, UserInfo* userInfo);
    void ResetSyncKey(const Json::Value& value);

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
