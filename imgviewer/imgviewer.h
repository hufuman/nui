#pragma once


class ImgViewer
{
public:
    ImgViewer();
    ~ImgViewer();

    void Show();
    void Destroy();
    HWND GetNative() const;

private:
    bool MsgCallback(NWindowBase*, UINT, WPARAM, LPARAM, LRESULT&);
    bool DrawCallback(NWindow*, NRender* render, const NRect& clipRect);
    void DrawTimerFunc();
    void OpenImage(LPCTSTR filePath);

private:
    NHolder drawTimerHolder_;
    NWindow window_;
    int frameIndex_;
    int frameCount_;
    NAutoPtr<NTimerSrv> timerSrv_;
    Base::NAutoPtr<NText> text_;
    Base::NAutoPtr<NFont> font_;
    Base::NAutoPtr<NImage> image_;
};
