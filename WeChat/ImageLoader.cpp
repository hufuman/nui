#include "StdAfx.h"
#include "ImageLoader.h"

#include "Config.h"
#include "HttpUtil.h"
#include "MiscUtil.h"

CImageLoader::CImageLoader(void)
{
    stopImageLoading_ = false;
    imageLoadingThread_ = NULL;
}

CImageLoader::~CImageLoader(void)
{
    Stop();
}

CImageLoader& CImageLoader::Get()
{
    static CImageLoader loader;
    return loader;
}

void CImageLoader::Stop()
{
    if(imageLoadingThread_ == NULL)
        return;
    stopImageLoading_ = true;
    ::WaitForSingleObject(imageLoadingThread_, INFINITE);
    imageLoadingThread_ = NULL;
}

void CImageLoader::LoadImage(NImage* image, LPCTSTR imgUrl, bool needRemoveAfterLoad)
{
    ImageLoadData data;
    data.image = image;
    data.imgUrl = imgUrl;
    data.imgName = GetImageName(imgUrl);
    data.needRemoveAfterLoad = needRemoveAfterLoad;

    {
        CAutoLocker autoLocker(&locker_);
        imageLoadDataQueue_.push(data);
    }

    if(imageLoadingThread_ == NULL)
        imageLoadingThread_ = reinterpret_cast<HANDLE>(_beginthreadex(0, 0, &CImageLoader::LoadImageThreadProc, reinterpret_cast<void*>(this), 0, 0));
}

UINT CImageLoader::LoadImageThreadProc(void* p)
{
    CImageLoader* pThis = reinterpret_cast<CImageLoader*>(p);

    ImageLoadData data;
    for(; !pThis->stopImageLoading_ ;)
    {
        if(pThis->stopImageLoading_)
            break;

        size_t size = 0;
        {
            CAutoLocker autoLocker(&pThis->locker_);
            size = pThis->imageLoadDataQueue_.size();
            if(size > 0)
            {
                data = pThis->imageLoadDataQueue_.front();
                pThis->imageLoadDataQueue_.pop();
            }
        }
        if(size == 0)
        {
            Sleep(500);
            continue;
        }

        if(data.image->RefCount() <= 1)
        {
            Sleep(10);
            continue;
        }

        data.imgPath = File::CombinePath(Config::Get().GetAppDataPath(_T("avatars")), data.imgName);
        if(!File::IsFileExists(data.imgName))
        {
            NString imgPath = data.imgPath + _T(".bak");
            if(!HttpUtil::GetFile(data.imgUrl, imgPath))
            {
                Sleep(10);
                continue;
            }
            ::MoveFile(imgPath, data.imgPath);
        }

        int result = SendUiProc(MakeDelegate(pThis, &CImageLoader::RenderImageProc), reinterpret_cast<void*>(&data));
        if(result == 0)
        {
            ::DeleteFile(data.imgPath);

            CAutoLocker autoLocker(&pThis->locker_);
            pThis->imageLoadDataQueue_.push(data);
        }
        else if(data.needRemoveAfterLoad)
        {
            ::DeleteFile(data.imgPath);
        }
    }

    return 0;
}

NString CImageLoader::GetImageName(LPCTSTR imgUrl)
{
    if(imgUrl == NULL || imgUrl[0] == 0)
        return _T("");
    return MiscUtil::GetMd5((unsigned char*)imgUrl, _tcslen(imgUrl)) + _T(".png");
}

LRESULT CImageLoader::RenderImageProc(void* p)
{
    ImageLoadData* data = reinterpret_cast<ImageLoadData*>(p);
    if(data->image->RefCount() <= 1)
        return 0;
    return data->image->LoadImage(data->imgPath) ? 1 : 0;
}
