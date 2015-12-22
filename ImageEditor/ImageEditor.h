#pragma once

class CImageEditor
{
public:
    CImageEditor(void);
    ~CImageEditor(void);

    void Show();

    bool MsgCallback(NWindowBase*, UINT, WPARAM, LPARAM, LRESULT&);
    bool PostDrawCallback(NBaseObj*, NEventData* eventData);

private:
    void OffsetImageIndex(HWND hWnd, int horzOffset, int vertOffset);
    void UpdateDraw(HWND hWnd, BOOL bReload);
    HWND CreateRealControl(HWND hWndParent, UINT uId, LPCTSTR szClassName, LPCTSTR szText, const NRect& rcEdit, DWORD dwStyle, DWORD dwExStyle);

    void OnDropFiles(HWND hWnd, HDROP hDrop);
    void SaveExtInfo(HWND hWnd, NString strPath);

private:
    NString filePath_;
    NAutoPtr<NImageDraw> image_;
    NAutoPtr<NShapeDraw> shape_;
    NAutoPtr<NWindow> window_;
};
