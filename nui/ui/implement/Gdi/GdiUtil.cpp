#include "stdafx.h"
#include "GdiUtil.h"

#include "ImageDC.h"

namespace Gdi
{
    CGdiHolder::CGdiHolder(HGDIOBJ obj, bool needDelete)
        : obj_(NULL), needDelete_(false)
    {
        Hold(obj, needDelete);
    }
    CGdiHolder::CGdiHolder()
        : obj_(NULL), needDelete_(false)
    {}

    CGdiHolder::~CGdiHolder()
    {
        Hold(NULL, true);
    }

    CGdiHolder::operator HGDIOBJ() const
    {
        return obj_;
    }

    void CGdiHolder::Hold(HGDIOBJ obj, bool needDelete)
    {
        needDelete_ = needDelete;
        obj_ = obj;
    }

    void CGdiHolder::Destroy()
    {
        if(obj_ && needDelete_)
        {
            ::DeleteObject(obj_);
            obj_ = NULL;
        }
    }

    CGdiSelector::CGdiSelector(HDC dc, HGDIOBJ obj, bool needDelete)
        : dc_(NULL), oldObj_(NULL), needDelete_(false)
    {
        Select(dc, obj, needDelete);
    }
    CGdiSelector::CGdiSelector()
        : dc_(NULL), oldObj_(NULL), needDelete_(false)
    {}

    CGdiSelector::~CGdiSelector()
    {
        Select(NULL, NULL, true);
    }

    void CGdiSelector::Destroy()
    {
        if(oldObj_ && dc_)
        {
            HGDIOBJ orgObj = ::SelectObject(dc_, oldObj_);
            if(needDelete_)
                ::DeleteObject(orgObj);
            dc_ = NULL;
            oldObj_ = NULL;
        }
    }

    void CGdiSelector::Select(HDC dc, HGDIOBJ obj, bool needDelete)
    {
        Destroy();
        needDelete_ = needDelete;
        if(dc == NULL || obj == NULL)
            return;

        dc_ = dc;
        oldObj_ = ::SelectObject(dc, obj);
    }

    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
    {
        UINT  num = 0;          // number of image encoders
        UINT  size = 0;         // size of the image encoder array in bytes

        Gdiplus::ImageCodecInfo* pImageCodecInfo = NULL;

        Gdiplus::GetImageEncodersSize(&num, &size);
        if(size == 0)
            return -1;  // Failure

        pImageCodecInfo = (Gdiplus::ImageCodecInfo*)(NMalloc(size));
        if(pImageCodecInfo == NULL)
            return -1;  // Failure

        Gdiplus::GetImageEncoders(num, size, pImageCodecInfo);

        for(UINT j = 0; j < num; ++j)
        {
            if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
            {
                *pClsid = pImageCodecInfo[j].Clsid;
                NFree(pImageCodecInfo);
                return j;  // Success
            }    
        }

        NFree(pImageCodecInfo);
        return -1;  // Failure
    }

    bool SaveBitmap(HBITMAP bitmap, LPCTSTR filePath)
    {
        bool result = false;

        // ImageDC imageDc(NULL, bitmap);
        // HPALETTE pal = (HPALETTE)::GetCurrentObject(imageDc, OBJ_PAL);

        Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromHBITMAP(bitmap, NULL);
        if(image != NULL)
        {
            if(image->GetLastStatus() == Gdiplus::Ok)
            {
                // µÃµ½CLSID
                nui::Base::NString fileExt(filePath);
                int pos = fileExt.LastIndexOf(_T("."));
                if(pos >= 0)
                {
                    fileExt = (LPCTSTR)fileExt + pos + 1;
                }

                nui::Base::NString format(_T("image/"));
                format += fileExt;

                // ±£´æ
                CLSID imageClsid = {0};
                if(GetEncoderClsid(format, &imageClsid) >= 0)
                {
                    result = (image->Save(filePath, &imageClsid, NULL) == Gdiplus::Ok);
                }
            }
            delete image;
        }

        return result;
    }
}