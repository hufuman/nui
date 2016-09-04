#include "StdAfx.h"
#include "WeChatLogic.h"

#include "JsonUtil.h"
#include "./json/reader.h"

#include <time.h>
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
    NString tmp;
    tmp.Format(_T("%u"), time(NULL));
    NString uuidUrl = _T("https://login.wx.qq.com/jslogin?appid=wx782c26e4c19acffb&redirect_uri=https%3A%2F%2Fwx.qq.com%2Fcgi-bin%2Fmmwebwx-bin%2Fwebwxnewloginpage&fun=new&lang=zh_CN&_=");
    uuidUrl += tmp;
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
    TCHAR tmpPath[1024];
    TCHAR filePath[1024];
    ::GetTempPath(_countof(tmpPath), tmpPath);
    ::GetTempFileName(tmpPath, _T("qr_"), rand(), filePath);
    *(_tcsrchr(filePath, _T('.'))) = 0;
    _tcsncat(filePath, _T(".png"), _countof(filePath));

    NString qrcodeUrl = _T("https://login.weixin.qq.com/qrcode/{uuid}");
    qrcodeUrl.Replace(_T("{uuid}"), uuid_);
    if(!HttpUtil::GetFile(qrcodeUrl, filePath))
        return NULL;

    return filePath;
}

int WeChatLogic::CheckScan()
{
    HttpUtil::HttpResult httpResult;
    NString url = _T("https://login.wx.qq.com/cgi-bin/mmwebwx-bin/login?loginicon=false&uuid={uuid}&tip=0&_=");
    url.Replace(_T("{uuid}"), uuid_);

    if(!HttpUtil::GetString(url, httpResult, 0))
        return 0;

    if(httpResult.text.IndexOf(_T("window.code=408;")) >= 0)
        return 1;

    if(httpResult.text.IndexOf(_T("window.code=201;")) >= 0)
        return 2;

    return 0;
}

int WeChatLogic::CheckLogin()
{
    HttpUtil::HttpResult httpResult;
    NString url = _T("https://login.wx.qq.com/cgi-bin/mmwebwx-bin/login?uuid={uuid}&tip=1&_=");
    url.Replace(_T("{uuid}"), uuid_);

    if(!HttpUtil::GetString(url, httpResult, 0))
        return 0;

    if(httpResult.text.IndexOf(_T("window.code=408;")) >= 0 || httpResult.text.IndexOf(_T("window.code=201;")) >= 0)
        return 1;

    if(httpResult.text.IndexOf(_T("window.code=200;")) < 0 || httpResult.text.IndexOf(_T("window.redirect_uri")) < 0)
        return 0;

    int position = 0;
    NString token;
    if(!httpResult.text.Tokenize(position, _T("\""), false, token)
        || !httpResult.text.Tokenize(position, _T("\""), false, token))
        return 0;

    redirectUrl_ = token;
    if(redirectUrl_.IsEmpty())
        return 0;

    position = redirectUrl_.LastIndexOf(_T("/"));
    baseUrl_ = redirectUrl_.SubString(0, position);
    return 2;
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
//         else if(name == _T("webwxuvid"))
//             uvid_ = value;
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
    url += ticket_ + _T("&r=1455625522&lang=");
    url += lang_;
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


    ParseUserInfo(user, &self_);
    sKey_ = JsonUtil::GetValue(value, "SKey");
    ResetSyncKey(value["SyncKey"]);

    return true;
}

bool WeChatLogic::FetchContracts()
{
    NString url= baseUrl_ + _T("/webwxgetcontact?lang=${lang}&pass_ticket=${ticket}&seq=0&skey=${sKey}&r=1455625522");
    url.Replace(_T("${lang}"), lang_);
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

    Json::Value memberList = value["MemberList"];
    for(Json::Value::iterator ite = memberList.begin(); ite != memberList.end(); ++ ite)
    {
        const Json::Value& contract = *ite;

        UserInfo* userInfo = new UserInfo();
        if(ParseUserInfo(contract, userInfo))
        {
            userInfoList_.push_back(userInfo);
            userInfoMap_.insert(std::make_pair(userInfo->userName, userInfo));
        }
        else
        {
            delete userInfo;
        }
    }
    return true;
}

bool WeChatLogic::QueryMsgExists(int& retcode, int& selector)
{
    NString tmp;
    tmp.Format(_T("%u"), time(NULL));
    NString url = _T("https://webpush.weixin.qq.com/cgi-bin/mmwebwx-bin/synccheck?skey=");
    url += sKey_;
    url += _T("&callback=jQuery183084135492448695_1420782130686&r=");
    url += tmp;
    url += _T("&sid=");
    url += sid_;
    url += _T("&uin=");
    url += uin_;
    url += _T("&deviceid=");
    url += _T("&synckey=");
    url += flatSyncKey_;

    HttpUtil::HttpResult httpResult;
    httpResult.cookie = cookie_;
    if(!HttpUtil::GetString(url, httpResult, 0))
        return false;

    if(httpResult.text.IndexOf(_T("window.synccheck=")) != 0)
        return false;

    const TCHAR retcodeTag[] = _T("retcode:\"");
    const TCHAR selectorTag[] = _T("selector:\"");
    int retcodePos = httpResult.text.IndexOf(retcodeTag);
    int selectorPos = httpResult.text.IndexOf(selectorTag);

    if(retcodePos >= 0)
        retcode = _ttoi(httpResult.text.SubString(retcodePos + _countof(retcodeTag) - 1));

    if(selectorPos >= 0)
        selector = _ttoi(httpResult.text.SubString(selectorPos + _countof(selectorTag) - 1));

    return retcode == 0 && selector != 0;
}

bool WeChatLogic::LoadMsgContent(std::list<WeChatMsg>& msgs)
{
    msgs.clear();

    NString tmp;
    tmp.Format(_T("%u"), time(NULL));
    NString url = baseUrl_ + _T("/webwxsync?sid={sid}&skey={skey}&r={rand}&lang={lang}&pass_ticket={ticket}");
    url.Replace(_T("{sid}"), sid_);
    url.Replace(_T("{skey}"), sKey_);
    url.Replace(_T("{lang}"), lang_);
    url.Replace(_T("{ticket}"), ticket_);
    url.Replace(_T("{rand}"), tmp);

    HttpUtil::HttpResult httpResult;
    NString data = _T("{\"BaseRequest\": {\"Uin\": \"{uin}\", \"Sid\": \"{sid}\"}, \"Skey\": \"{skey}\", \"SyncKey\": {syncKey}, \"rr\": {rand}}");
    data.Replace(_T("{uin}"), uin_);
    data.Replace(_T("{sid}"), sid_);
    data.Replace(_T("{skey}"), sKey_);
    data.Replace(_T("{syncKey}"), syncKey_);
    data.Replace(_T("{rand}"), tmp);
    std::string body = t2utf8(data);
    httpResult.cookie = cookie_;
    if(!HttpUtil::PostString(url, (LPVOID)(body.c_str()), body.size(), httpResult))
        return false;

    Json::Reader reader;
    Json::Value value;
    if(!reader.parse(t2utf8(httpResult.text), value))
        return false;

    ResetSyncKey(value["SyncKey"]);

    int msgCount = value["AddMsgCount"].asInt();
    if(msgCount <= 0)
        return false;

    Json::Value msgList = value["AddMsgList"];

    WeChatMsg msg;
    for(Json::Value::iterator ite = msgList.begin(); ite != msgList.end(); ++ ite)
    {
        const Json::Value& msgObj = *ite;

        msg.Reset();
        msg.MsgType = (WeChatMsgType)JsonUtil::GetIntValue(msgObj, "MsgType");
        if(msg.MsgType != WeChatMsgText)
            continue;

        msg.Content = JsonUtil::GetValue(msgObj, "Content");
        msg.FromUserName = JsonUtil::GetValue(msgObj, "FromUserName");
        msg.ToUserName = JsonUtil::GetValue(msgObj, "ToUserName");
        msg.MsgId = JsonUtil::GetValue(msgObj, "MsgId");

        msgs.push_back(msg);
    }

    return false;
}

WeChatLogic::UserInfoList& WeChatLogic::GetUserInfoList()
{
    return userInfoList_;
}

bool WeChatLogic::ParseUserInfo(const Json::Value& user, UserInfo* userInfo)
{
    userInfo->userName = JsonUtil::GetValue(user, "UserName");
    userInfo->nickName = JsonUtil::GetValue(user, "NickName");
    userInfo->displayName = JsonUtil::GetValue(user, "DisplayName");
    userInfo->remarkName = JsonUtil::GetValue(user, "RemarkName");
    userInfo->contactFlag_ = JsonUtil::GetIntValue(user, "ContactFlag");
    userInfo->sex_ = JsonUtil::GetIntValue(user, "Sex");
    self_.signature_ = JsonUtil::GetValue(user, "Signature");

    userInfo->searchPinyins_ = JsonUtil::GetValue(user, "KeyWord") + _T("|")
        + JsonUtil::GetValue(user, "PYInitial") + _T("|")
        + JsonUtil::GetValue(user, "PYQuanPin") + _T("|")
        + JsonUtil::GetValue(user, "RemarkPYInitial") + _T("|")
        + JsonUtil::GetValue(user, "RemarkPYQuanPin") + _T("|");

    userInfo->headImgUrl = _T("https://wx.qq.com");
    userInfo->headImgUrl += JsonUtil::GetValue(user, "HeadImgUrl");
    return true;
}

void WeChatLogic::ResetSyncKey(const Json::Value& value)
{
    if(value.type() != Json::objectValue)
        return;
    if(JsonUtil::GetIntValue(value, "Count") <= 0)
        return;
    NString tmpSyncKey = utf82t(value.toStyledString().c_str());
    if(tmpSyncKey.GetLength() <= 2)
        return;
    syncKey_ = tmpSyncKey;
    Json::Value list = value["List"];
    flatSyncKey_ = _T("");
    NString temp;
    for(Json::Value::iterator ite = list.begin(); ite != list.end(); ++ ite)
    {
        Json::Value& v = *ite;
        flatSyncKey_ += temp.Format(_T("%d"), JsonUtil::GetIntValue(v, "Key"));
        flatSyncKey_ += _T("_");
        flatSyncKey_ += temp.Format(_T("%d"), JsonUtil::GetIntValue(v, "Val"));
        flatSyncKey_ += _T("%7C");
    }
    flatSyncKey_ = flatSyncKey_.SubString(0, flatSyncKey_.GetLength() - 3);
}
