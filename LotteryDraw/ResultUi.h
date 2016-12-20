#pragma once

class ResultUi
{
public:
    ResultUi(void);
    ~ResultUi(void);

    void Show(const NString& result);

private:
    bool OnWindowCreated(Base::NBaseObj* source, NEventData* eventData);

private:
    NInstPtr<NWindow> window_;
    NString result_;
};
