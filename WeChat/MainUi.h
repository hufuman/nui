#pragma once





class MainUi
{
public:
    MainUi(void);
    ~MainUi(void);

    void Show();

private:
    bool OnWindowCreated(Base::NBaseObj* source, NEventData* eventData);

private:
    NInstPtr<NWindow> window_;
};

