#pragma once



#include "HttpUtil.h"

class UserInfo
{
public:
    NString userName;
    NString nickName;
    NString remarkName;
    NString headImgUrl;
};

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
    bool CheckScan();
    bool CheckLogin();
    bool FetchUserAuth();
    bool FetchUserInfo();

    bool FetchContracts();

    typedef std::list<UserInfo*> UserInfoList;
    const UserInfoList& GetUserInfoList() const;

private:
    void ResetSyncKey(Json::Value& value);

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
    NString syncKey_;

    // UserName => UserInfo
    typedef std::map<NString, UserInfo*> UserInfoMap;

    UserInfoList userInfoList_;
    UserInfoMap userInfoMap_;

};
