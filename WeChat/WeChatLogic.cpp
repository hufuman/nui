#include "StdAfx.h"
#include "WeChatLogic.h"

#include "./json/reader.h"

#include <wininet.h>

#pragma warning(disable: 4996)


namespace
{
    NString g_BaseUrl = _T("https://wx.qq.com/cgi-bin/mmwebwx-bin");
}

WeChatLogic::WeChatLogic(void)
{
}

WeChatLogic::~WeChatLogic(void)
{
}


WeChatLogic& WeChatLogic::Get()
{
    static WeChatLogic logic;
    return logic;
}

bool WeChatLogic::DownloadUuid()
{
    HttpUtil::HttpResult httpResult;

    // fetch uuid
    LPCTSTR uuidUrl = _T("https://login.weixin.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https%3A%2F%2Fwx.qq.com%2Fcgi-bin%2Fmmwebwx-bin%2Fwebwxnewloginpage&fun=new&lang=zh_CN&_=1388994062250");
    if(!HttpUtil::GetString(uuidUrl, httpResult, 0))
        return false;

    NString token;
    int position = 0;
    if(!httpResult.text.Tokenize(position, _T("\""), false, token)
        || !httpResult.text.Tokenize(position, _T("\""), false, token))
        return false;

    uuid_ = token;
    return true;
}

NString WeChatLogic::DownloadQrCode()
{
    HttpUtil::HttpResult httpResult;

    // download qrcode
    TCHAR filePath[1024];
    ::GetTempPath(_countof(filePath), filePath);
    _tcsncat(filePath, _T("\\qrcode.png"), _countof(filePath));

    NString qrcodeUrl = _T("https://login.weixin.qq.com/qrcode/{uuid}?t=webwx");
    qrcodeUrl.Replace(_T("{uuid}"), uuid_);
    if(!HttpUtil::GetFile(qrcodeUrl, filePath))
        return NULL;

    return filePath;
}

bool WeChatLogic::CheckScan()
{
    HttpUtil::HttpResult httpResult;
    NString url = _T("https://login.weixin.qq.com/cgi-bin/mmwebwx-bin/login?uuid={uuid}&tip=1&_=");
    url.Replace(_T("{uuid}"), uuid_);

    if(!HttpUtil::GetString(url, httpResult, 0))
        return false;

    if(httpResult.text.IndexOf(_T("window.code=201;")) < 0)
        return false;

    char data[1024] = "{\"BaseRequest\":{\"Uin\":0,\"Sid\":0},\"Count\":1,\"List\":[{\"Type\":1,\"Text\":\"/cgi-bin/mmwebwx-bin/login, First Request Success, uuid: ";
    strncat(data, t2utf8(uuid_).c_str(), _countof(data));
    strncat(data, "\"}]}", _countof(data));
    HttpUtil::PostString(g_BaseUrl + _T("/webwxstatreport?type=1&r=1455625520"), data, strlen(data), httpResult);

    strncpy(data, "{\"BaseRequest\":{\"Uin\":0,\"Sid\":0},\"Count\":1,\"List\":[{\"Type\":1,\"Text\":\"/cgi-bin/mmwebwx-bin/login, Second Request Success, uuid: ", _countof(data));
    strncat(data, t2utf8(uuid_).c_str(), _countof(data));
    strncat(data, "\"}]}", _countof(data));
    HttpUtil::PostString(g_BaseUrl + _T("/webwxstatreport?type=1&r=1455625520"), data, strlen(data), httpResult);

    return true;
}

bool WeChatLogic::CheckLogin()
{
    HttpUtil::HttpResult httpResult;
    NString url = _T("https://login.weixin.qq.com/cgi-bin/mmwebwx-bin/login?uuid={uuid}&tip=1&_=");
    url.Replace(_T("{uuid}"), uuid_);

    if(!HttpUtil::GetString(url, httpResult, 0))
        return false;

    if(httpResult.text.IndexOf(_T("window.redirect_uri")) < 0)
        return false;

    int position = 0;
    NString token;
    if(!httpResult.text.Tokenize(position, _T("\""), false, token)
        || !httpResult.text.Tokenize(position, _T("\""), false, token))
        return false;

    redirectUrl_ = token;
    if(redirectUrl_.IsEmpty())
        return false;

    position = redirectUrl_.LastIndexOf(_T("/"));
    baseUrl_ = redirectUrl_.SubString(0, position);
    return true;
}

bool WeChatLogic::FetchUserAuth()
{
    HttpUtil::HttpResult httpResult;
    httpResult.NeedHeader(_T("Location"));
    httpResult.NeedHeader(_T("Set-Cookie"));
    if(!HttpUtil::GetString(redirectUrl_ + _T("&func=new"), httpResult, INTERNET_FLAG_NO_AUTO_REDIRECT))
        return false;

    NString cookie = httpResult.GetHeader(_T("Set-Cookie"));

    int position = 0;
    NString token;
    NString name;
    NString value;
    for(; cookie.Tokenize(position, _T("<->"), false, token); )
    {
        int start = token.IndexOf(_T("="));
        int end = token.IndexOf(_T(";"));
        name = token.SubString(0, start);
        value = token.SubString(start + 1, end - start - 1);

        if(name == _T("wxuin"))
            uin_ = value;
        else if(name == _T("webwx_data_ticket"))
            ticket_ = value;
        else if(name == _T("wxsid"))
            sid_ = value;
        else if(name == _T("wxloadtime"))
            loadTime_ = value;
        else if(name == _T("webwxuvid"))
            uvid_ = value;
        else if(name == _T("mm_lang"))
            lang_ = value;
    }

    cookie_ = _T("wxuin=");
    cookie_ += uin_;
    cookie_ += _T("; wxsid=");
    cookie_ += sid_;
    cookie_ += _T("; wxloadtime=");
    cookie_ += loadTime_;
    cookie_ += _T("; mm_lang=");
    cookie_ += lang_;
    cookie_ += _T("; webwx_data_ticket=");
    cookie_ += ticket_;
    cookie_ += _T("; webwxuvid=");
    cookie_ += uvid_;

    return true;
}

bool WeChatLogic::FetchUserInfo()
{
    HttpUtil::HttpResult httpResult;
    NString url = baseUrl_ + _T("/webwxinit?pass_ticket=");
    url += ticket_ + _T("&r=1455625522");
    NString data = _T("{\"BaseRequest\":{\"Uin\":\"{uin}\",\"Sid\":\"{sid}\",\"Skey\":\"\",\"DeviceID\":\"{deviceId}\"}}");
    data.Replace(_T("{uin}"), uin_);
    data.Replace(_T("{sid}"), sid_);
    std::string body = t2utf8(data);
    httpResult.cookie = cookie_;
    if(!HttpUtil::PostString(url, (LPVOID)(body.c_str()), body.size(), httpResult))
        return false;

    Json::Reader reader;
    Json::Value value;
    if(!reader.parse(t2utf8(httpResult.text), value))
        return false;

    Json::Value user = value["User"];

    self_.userName = utf82t(user["UserName"].asString().c_str());
    self_.nickName = utf82t(user["NickName"].asString().c_str());
    self_.remarkName = utf82t(user["RemarkName"].asString().c_str());
    self_.headImgUrl = utf82t(user["HeadImgUrl"].asString().c_str());
    sKey_ = utf82t(value["SKey"].asString().c_str());
    ResetSyncKey(value["SyncKey"]);

    return true;
}

bool WeChatLogic::FetchContracts()
{
    NString url= baseUrl_ + _T("/webwxgetcontact?lang=zh_CN&pass_ticket=${ticket}&seq=0&skey=${sKey}&r=1455625522");
    url.Replace(_T("${ticket}"), ticket_);
    url.Replace(_T("${sKey}"), sKey_);
    HttpUtil::HttpResult httpResult;
    httpResult.cookie = cookie_;
    if(!HttpUtil::GetString(url, httpResult, 0))
        return false;

    Json::Value value;
    Json::Reader reader;
    if(!reader.parse(t2utf8(httpResult.text), value))
        return false;

    UserInfo userInfo;
    Json::Value memberList = value["MemberList"];
    for(Json::Value::iterator ite = memberList.begin(); ite != memberList.end(); ++ ite)
    {
        const Json::Value& contract = *ite;
        userInfo.userName = utf82t(contract["UserName"].asString().c_str());
        userInfo.nickName = utf82t(contract["NickName"].asString().c_str());
        userInfo.remarkName = utf82t(contract["RemarkName"].asString().c_str());
        userInfo.headImgUrl = utf82t(contract["HeadImgUrl"].asString().c_str());
        userInfoMap_.insert(std::make_pair(userInfo.userName, userInfo));
    }
    return true;
}

void WeChatLogic::ResetSyncKey(Json::Value& value)
{
    syncKey_ = _T("");
    for(Json::Value::iterator ite = value.begin(); ite != value.end(); ++ ite)
    {
        syncKey_ += utf82t(ite.key().asString().c_str());
        syncKey_ += _T("_");
        Json::Value& v = *ite;
        std::string s = v.toStyledString();
        syncKey_ += utf82t(s.c_str());
        syncKey_ += _T("%7C");
    }
    syncKey_ = syncKey_.SubString(0, syncKey_.GetLength() - 3);
}
