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
    void PaintTest();

private:
    NAutoPtr<NWindow> window_;
};
