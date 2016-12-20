#include "stdafx.h"
#include "LoginUi.h"

#include "WeChatLogic.h"


LoginUi::LoginUi(void) : window_(MemToolParam)
{
    stop_ = false;
    loginOk_ = false;
    loginThread_ = NULL;
}

LoginUi::~LoginUi(void)
{
}

bool LoginUi::Show()
{
    window_->SetStyle(WindowStyle::Top);
    window_->WindowCreatedEvent.AddHandler(this, &LoginUi::OnWindowCreated);
    window_->SetVisible(true);
    window_->DoModalWithStyle(NULL, _T("@LoginUi:LoginUi"));

    if(loginThread_ != NULL)
    {
        HttpUtil::StopAllRequest();
        stop_ = true;
        ::WaitForSingleObject(loginThread_, INFINITE);
        ::CloseHandle(loginThread_);
        loginThread_ = NULL;
    }

    return loginOk_;
}

bool LoginUi::OnWindowCreated(Base::NBaseObj* source, NEventData* eventData)
{
    NFrame* rootFrame = window_->GetRootFrame();
    statusLabel_ = rootFrame->GetChildById<NLabel*>(_T("status"));
    qrcode_ = rootFrame->GetChildById<NImage*>(_T("qrcode"));
    btnRetry_ = rootFrame->GetChildById<NButton*>(_T("btnRetry"));
    btnRetry_->ClickEvent.AddHandler(this, &LoginUi::OnBtnRetry);

    DoLogin();
    return false;
}

bool LoginUi::OnBtnRetry(Base::NBaseObj* source, NEventData* eventData)
{
    DoLogin();
    return false;
}

void LoginUi::DoLogin()
{
    btnRetry_->SetVisible(false);
    if(loginThread_)
        ::CloseHandle(loginThread_);
    loginThread_ = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &LoginUi::ThreadProc, reinterpret_cast<void*>(this), 0, 0));
}

unsigned int LoginUi::ThreadProc(void* data)
{
    LoginUi* pThis = reinterpret_cast<LoginUi*>(data);

    bool result = false;
    NString errMsg;
    for(;!pThis->stop_;)
    {
        pThis->statusLabel_->SetText(_T("@LoginUi:queryQrCode"));
        pThis->qrcode_->LoadImage(_T("@skin:Common\\loading.gif"));
        if(!WeChatLogic::Get().DownloadUuid())
        {
            errMsg = _T("@LoginUi:errUuid");
            break;
        }

        NString qrCodeFilePath = WeChatLogic::Get().DownloadQrCode();
        if(qrCodeFilePath.IsEmpty())
        {
            errMsg = _T("@LoginUi:errQrCode");
            break;
        }

        pThis->qrcode_->LoadImage(qrCodeFilePath);

        // check scan
        result = false;
        pThis->statusLabel_->SetText(_T("@LoginUi:scanQrCode"));
        DWORD dwTickCount = ::GetTickCount();
        int scanResult = 1;
        int retryCount = 0;
        for(; !pThis->stop_ && retryCount < 4; )
        {
            scanResult = WeChatLogic::Get().CheckScan();
            if(scanResult == 2)
                break;
            ::Sleep(500);
            if(scanResult == 0)
            {
                ++ retryCount;
                continue;
            }
            retryCount = 0;
        }
        if(scanResult != 2)
        {
            continue;
        }

        // check login
        result = false;
        pThis->statusLabel_->SetText(_T("@LoginUi:checkLogin"));
        scanResult = 1;
        retryCount = 0;
        for(; !pThis->stop_ && retryCount < 4; )
        {
            scanResult = WeChatLogic::Get().CheckLogin();
            if(scanResult == 2)
                break;
            ::Sleep(500);
            if(scanResult == 0)
            {
                ++ retryCount;
                continue;
            }
            retryCount = 0;
        }
        if(scanResult != 2)
        {
            continue;
        }

        // user auth
        result = false;
        pThis->statusLabel_->SetText(_T("@LoginUi:fetchUserAuth"));
        for(int i=0; !pThis->stop_ && !result && i<5; ++ i)
        {
            result = WeChatLogic::Get().FetchUserAuth();
            ::Sleep(500);
        }
        if(!result)
        {
            errMsg = _T("@LoginUi:errUserAuth");
            break;
        }

        // user info
        result = false;
        pThis->statusLabel_->SetText(_T("@LoginUi:fetchUserInfo"));
        for(int i=0; !pThis->stop_ && !result && i<5; ++ i)
        {
            result = WeChatLogic::Get().FetchUserInfo();
            ::Sleep(500);
        }
        if(!result)
        {
            errMsg = _T("@LoginUi:errUserInfo");
            break;
        }

        // contracts
        result = false;
        pThis->statusLabel_->SetText(_T("@LoginUi:fetchContracts"));
        for(int i=0; !pThis->stop_ && !result && i<5; ++ i)
        {
            result = WeChatLogic::Get().FetchContracts();
            ::Sleep(500);
        }
        if(!result)
        {
            errMsg = _T("@LoginUi:errContracts");
            break;
        }

        result = true;
        break;
    }

    if(pThis->stop_)
        return 0;

    if(result)
    {
        pThis->loginOk_ = result;
        pThis->window_->Destroy();
    }
    else
    {
        pThis->statusLabel_->SetText(errMsg);
        pThis->btnRetry_->SetVisible(true);
        pThis->qrcode_->LoadImage(_T("@skin:Common\\loading.gif"));
    }

    return 0;
}
