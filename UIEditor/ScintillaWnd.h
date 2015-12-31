#pragma once


class CScintillaWnd
{
public:
    CScintillaWnd(void);
    ~CScintillaWnd(void);

public:
    bool Create(HWND hWndParent, const RECT& rcWnd, DWORD dwStyle = WS_VISIBLE | WS_CHILD);
    bool OnNotify(WPARAM wParam, LPARAM lParam, LRESULT& lResult);
    HWND GetHandle() const;

    void SetKeywords(const char* keywords);

    bool Open(const NString& filePath);
    bool Save(const NString& filePath);
    void Clear();

private:
    bool LoadModule();
    bool InitEditor();

    LRESULT SendEditorMsg(UINT msg, WPARAM wParam = 0, LPARAM lParam = 0);
    void SetAStyle(int style, COLORREF fore, COLORREF back = RGB(255, 255, 255), int size = -1, const char *face = NULL);

private:
    HWND m_hWnd;
};
