#pragma once

class CControlTest
{
public:
    CControlTest(void);
    ~CControlTest(void);

    void Test();

private:
    bool OnButtonClicked(NFrame* pButton, const Base::NPoint& point);
    void OnEditTextChanged(NEdit* pEdit);
    void OnScrollEvent(NScroll* pScroll, int scrollPos);
    bool PaintTest(NWindow* window, NRender* render, HRGN clipRgn);

private:
    NAutoPtr<NWindow> window_;
    NAutoPtr<NScroll> horzScroll_;
    NAutoPtr<NScroll> vertScroll_;
    NAutoPtr<NEdit> editPos_;
    NAutoPtr<NEdit> editRange_;
};
