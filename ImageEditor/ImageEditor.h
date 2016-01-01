#pragma once

class CImageEditor
{
public:
    CImageEditor(void);
    ~CImageEditor(void);

    void Show();

private:

    void ListenEvents();

    bool MsgCallback(NWindowBase*, UINT, WPARAM, LPARAM, LRESULT&);
    bool PostDrawCallback(NBaseObj*, NEventData* eventData);

    void UpdateDraw(HWND hWnd, BOOL bReload);

    void OnDropFiles(HWND hWnd, HDROP hDrop);
    void SaveExtInfo(HWND hWnd, NString strPath);

    bool OnImageIndexChanged(NBaseObj*, NEventData*);
    bool OnBtnBrowse(NBaseObj*, NEventData*);
    bool OnBtnSave(NBaseObj*, NEventData*);
    bool OnParamChanged(NBaseObj*, NEventData*);
    bool OnDrawTypeChanged(NBaseObj*, NEventData*);

private:
    NString filePath_;
    NAutoPtr<NImageDraw> image_;
    NAutoPtr<NWindow> window_;
};
