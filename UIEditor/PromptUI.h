#pragma once

class CPromptUI
{
public:
    CPromptUI(void);
    ~CPromptUI(void);

    bool DoModal(HWND hWnd, NString& styleName);

protected:
    // events
    bool onWindowCreated(NBaseObj*, NEventData*);
    bool onBtnPreview(NBaseObj*, NEventData*);

private:
    NWindow* window_;
    NFrame* rootFrame_;
    NString styleName_;
    bool needPreview_;
};
