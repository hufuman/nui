#pragma once


#include "Locker.h"
#include <queue>

class CImageLoader
{
    CImageLoader();
    CImageLoader(const CImageLoader&);
    CImageLoader& operator = (const CImageLoader&);
public:
    ~CImageLoader(void);

    static CImageLoader& Get();

public:
    void LoadImage(NImage* image, LPCTSTR imgUrl, bool needRemoveAfterLoad);
    void Stop();

private:
    static UINT WINAPI LoadImageThreadProc(void*);

    NString GetImageName(LPCTSTR imgUrl);
    LRESULT RenderImageProc(void*);

private:

    struct ImageLoadData
    {
        NAutoPtr<NImage> image;
        NString imgUrl;
        NString imgName;
        NString imgPath;
        bool needRemoveAfterLoad;
    };

    typedef std::queue<ImageLoadData> ImageLoadDataQueue;
    ImageLoadDataQueue imageLoadDataQueue_;

    bool stopImageLoading_;
    HANDLE imageLoadingThread_;

    CSectionLocker locker_;
};
