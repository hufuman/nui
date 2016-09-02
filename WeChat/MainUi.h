#pragma once





class MainUi
{
public:
    MainUi(void);
    ~MainUi(void);

    // whether need to show login
    bool Show();

private:
    bool OnWindowCreated(Base::NBaseObj* source, NEventData* eventData);
    void LoadMsgThreadProc(bool& stopping);

private:
    bool needShowLogin_;
    NThread loadMsgThread_;
    NInstPtr<NWindow> window_;
};

