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

private:
    NHolder drawTimerHolder_;
    NWindow window_;
    int frameIndex_;
    int frameCount_;
    NAutoPtr<NTimerSrv> timerSrv_;
    Base::NAutoPtr<NText> text_;
    Base::NAutoPtr<NFont> font_;
    Base::NAutoPtr<NImage> image_;

    NString fileDlgExts_;
    NArrayT<NString> supportedForamts_;
};
