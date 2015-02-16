#pragma once

class NWnd
{
public:
    NWnd(void);
    ~NWnd(void);

    bool Create(HWND parentWindow);
    void SetVisible(bool visible);
    void SetText(LPCTSTR text);
    void SetSize(int width, int height);
    void CenterWindow(HWND relativeWindow);
    virtual void Destroy();
    HWND GetNative() const;
    void SetMsgFilterCallback(nui::Ui::MsgFilterCallback callback);

protected:
    void DoPaint();
    void UpdateWindowCorner(int width, int height);

    bool WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

protected:
    nui::Ui::NWindowBase window_;
    nui::Ui::MsgFilterCallback callback_;
};
