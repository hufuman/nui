#pragma once

class CImageEditor
{
public:
    CImageEditor(void);
    ~CImageEditor(void);

    void Show();

    bool MsgCallback(NWindowBase*, UINT, WPARAM, LPARAM, LRESULT&);
    bool DrawCallback(NWindow*, NRender* render, const NRect& clipRect);

private:
    void UpdateDraw(NWindowBase* window, BOOL bReload);
    HWND CreateRealControl(HWND hWndParent, UINT uId, LPCTSTR szClassName, LPCTSTR szText, const NRect& rcEdit, DWORD dwStyle, DWORD dwExStyle);

    void SaveExtInfo(HWND hWnd, NString strPath);

private:
    NString filePath_;
    NAutoPtr<NImageDraw> image_;
    NAutoPtr<NShapeDraw> shape_;
    NAutoPtr<NWindow> window_;
};
