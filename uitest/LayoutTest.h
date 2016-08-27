#pragma once

class CLayoutTest
{
public:
    CLayoutTest(void);
    ~CLayoutTest(void);

    void Test();

private:
    bool OnWindowCreated(NBaseObj*, NEventData* eventData);

    bool OnBtnAddClicked(NBaseObj* pButton, NEventData* eventData);
    bool OnBtnRemoveClicked(NBaseObj* pButton, NEventData* eventData);

private:
    NAutoPtr<NWindow> window_;
    int count_;
};
