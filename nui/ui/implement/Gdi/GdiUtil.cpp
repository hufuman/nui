#include "stdafx.h"
#include "GdiUtil.h"

#include "ImageDC.h"

namespace
{
    /*
        Image Ext Info
        Version Format
        1       |StartTag Version ExtInfo EndTag|
    */
    const DWORD g_dwImageExtInfoStartTag = 0xF28D6A40;
    const DWORD g_dwImageExtInfoEndTag = 0xDA64F190;

}

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
        Destroy();
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
                nui::Base::NString fileExt(filePath);
                int pos = fileExt.LastIndexOf(_T("."));
                if(pos >= 0)
                {
                    fileExt = (LPCTSTR)fileExt + pos + 1;
                }

                nui::Base::NString format(_T("image/"));
                format += fileExt;

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

    bool GetImageData(const BYTE* pData, DWORD dwSize, nui::Ui::stImageExtInfo& extInfo)
    {
        const DWORD dwExtBodySize = sizeof(g_dwImageExtInfoStartTag) + sizeof(DWORD) + sizeof(nui::Ui::stImageExtInfo) + sizeof(g_dwImageExtInfoEndTag);
        if(dwSize < dwExtBodySize)
            return false;
        const BYTE* pStart = pData + dwSize - dwExtBodySize;
        if(*((DWORD*)pStart) != g_dwImageExtInfoStartTag
            || *((DWORD*)pStart + 1) != 1
            || *((DWORD*)(pData + dwSize) - 1) != g_dwImageExtInfoEndTag)
        {
            return false;
        }
        memcpy(&extInfo, (LPCVOID)(pStart + sizeof(DWORD) + sizeof(DWORD)), sizeof(extInfo));
        return true;
    }

    void SetImageData(const nui::Ui::stImageExtInfo& extInfo, std::string& strData)
    {
        // |StartTag Version ExtInfo EndTag|
        DWORD dwVersion = 1;
        strData = "";
        strData.append((const char*)&g_dwImageExtInfoStartTag, sizeof(g_dwImageExtInfoStartTag));
        strData.append((const char*)&dwVersion, sizeof(dwVersion));
        strData.append((const char*)&extInfo, sizeof(extInfo));
        strData.append((const char*)&g_dwImageExtInfoEndTag, sizeof(g_dwImageExtInfoEndTag));

        const DWORD dwExtBodySize = sizeof(g_dwImageExtInfoStartTag) + sizeof(DWORD) + sizeof(nui::Ui::stImageExtInfo) + sizeof(g_dwImageExtInfoEndTag);
        NVerify(dwExtBodySize == strData.size(), _T("Wrong data"));
    }

    bool UpdateImageData(LPCTSTR filePath, const nui::Ui::stImageExtInfo& extInfo)
    {
        HANDLE fileHandle = ::CreateFile(filePath, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
        if(fileHandle == NULL)
            return false;

        const DWORD dwExtBodySize = sizeof(g_dwImageExtInfoStartTag) + sizeof(DWORD) + sizeof(nui::Ui::stImageExtInfo) + sizeof(g_dwImageExtInfoEndTag);
        DWORD dwSize = ::GetFileSize(fileHandle, NULL);
        bool result = false;
        if(dwSize > dwExtBodySize)
        {
            BYTE byData[dwExtBodySize];
            if(::SetFilePointer(fileHandle, 0 - dwExtBodySize, 0, FILE_END) > 0)
            {
                std::string strData;
                SetImageData(extInfo, strData);

                DWORD dwRead;
                DWORD dwWritten;
                nui::Ui::stImageExtInfo tmpExtInfo;
                if(::ReadFile(fileHandle, byData, dwExtBodySize, &dwRead, 0)
                    && dwExtBodySize == dwRead
                    && GetImageData(byData, dwExtBodySize, tmpExtInfo))
                {
                    // Replace existing
                    result = ::SetFilePointer(fileHandle, 0 - dwExtBodySize, 0, FILE_END) > 0
                        && ::WriteFile(fileHandle, &strData[0], strData.size(), &dwWritten, 0)
                        && dwWritten == strData.size()
                        && ::SetEndOfFile(fileHandle);
                }
                else
                {
                    // Append New
                    result = ::SetFilePointer(fileHandle, 0, 0, FILE_END) > 0
                        && ::WriteFile(fileHandle, &strData[0], strData.size(), &dwWritten, 0)
                        && dwWritten == strData.size()
                        && ::SetEndOfFile(fileHandle);
                }
            }
        }
        ::CloseHandle(fileHandle);
        return result;
    }
}
