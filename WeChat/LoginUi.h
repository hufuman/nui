#pragma once

class LoginUi
{
public:
    LoginUi(void);
    ~LoginUi(void);

public:
    bool Show();

private:
    void DoLogin();
    bool OnWindowCreated(Base::NBaseObj* source, NEventData* eventData);
    bool OnBtnRetry(Base::NBaseObj* source, NEventData* eventData);
    static unsigned int WINAPI ThreadProc(void* data);

private:
    NInstPtr<NWindow> window_;
    NLabel* statusLabel_;
    NImage* qrcode_;
    NButton* btnRetry_;

    bool loginOk_;
    HANDLE loginThread_;
};
