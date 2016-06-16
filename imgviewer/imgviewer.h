#pragma once

#include "NFileFinder.h"


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
    void DrawTimerFunc();
    bool OpenImage(LPCTSTR filePath, bool reset);
    NString GetFileDlgExts();
    void GetProperSize(NSize& size);

    bool PostDrawCallback(NBaseObj* baseObj, NEventData* eventData);
    bool OnKeyDown(NBaseObj* baseObj, NEventData* eventData);

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
    NString text_;
    Base::NAutoPtr<NTextAttr> textAttr_;
    Base::NAutoPtr<NFont> font_;
    Base::NAutoPtr<NImageDraw> image_;

    double rate_;

    NFileFinder fileFinder_;

    NString fileDlgExts_;
    NArrayT<NString> supportedForamts_;
};
