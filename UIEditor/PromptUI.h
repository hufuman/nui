#pragma once

class CPromptUI
{
public:
    CPromptUI(void);
    ~CPromptUI(void);

    NString DoModal(HWND hWnd);

protected:
    // events
    bool onWindowCreated(NBaseObj*, NEventData*);
    bool onBtnPreview(NBaseObj*, NEventData*);

private:
    NWindow* window_;
    NFrame* rootFrame_;
    NString styleName_;
};
