#pragma once


class CControlTest
{
public:
    CControlTest(void);
    ~CControlTest(void);

    void Test();

private:
    bool OnButtonClicked(NBaseObj* pButton, NEventData* eventData);
    bool OnEditTextChanged(NBaseObj* pButton, NEventData* eventData);
    bool OnScrollEvent(NBaseObj* baseObj, NEventData* eventData);
    bool PaintTest(NBaseObj* baseObj, NEventData* eventData);

    void AddLine(NList* pList);

private:
    NAutoPtr<NWindow> window_;
    NAutoPtr<NScroll> horzScroll_;
    NAutoPtr<NScroll> vertScroll_;
    NAutoPtr<NEdit> editPos_;
    NAutoPtr<NEdit> editPage_;
    NAutoPtr<NEdit> editRange_;
    NAutoPtr<NList> list_;
};
