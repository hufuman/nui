#pragma once


class ImgViewer
{
public:
    ImgViewer();
    ~ImgViewer();

    void Show(LPCTSTR filePath);
    void Destroy();
    HWND GetNative() const;
    void ShowPrev(HWND hWnd, LPCTSTR filePath);

    const NArrayT<NString>& GetSupportedFormats() const;

private:
    bool MsgCallback(NWindowBase*, UINT, WPARAM, LPARAM, LRESULT&);
    bool DrawCallback(NWindow*, NRender* render, const NRect& clipRect);
    void DrawTimerFunc();
    bool OpenImage(LPCTSTR filePath);
    NString GetFileDlgExts();
    void GetProperSize(NSize& size);

    // message handler
    void OnCopyData(COPYDATASTRUCT* cds);
    void OnDropFiles(HDROP hDrop);
    void OnMouseWheel(short delta);

    void InitControls();

private:
    NHolder drawTimerHolder_;
    NAutoPtr<NWindow> window_;
    int frameIndex_;
    int frameCount_;
    NAutoPtr<NTimerSrv> timerSrv_;
    Base::NAutoPtr<NText> text_;
    Base::NAutoPtr<NFont> font_;
    Base::NAutoPtr<NImageDraw> image_;

    int rate_;

    NString fileDlgExts_;
    NArrayT<NString> supportedForamts_;
};
