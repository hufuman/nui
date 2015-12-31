#pragma once


#include "../../../Base/DataTypes.h"


class MemDC
{
    MemDC(const MemDC&);
    MemDC& operator = (const MemDC&);
public:
    MemDC()
    {
        memDC_ = NULL;
        oldBitmap_ = NULL;
        bitmap_ = NULL;
    }
    ~MemDC()
    {
        Destroy();
    }

    operator HDC()
    {
        return memDC_;
    }

    void Init(HDC hDc, const nui::Base::NRect& rcPaint, BYTE alphaValue)
    {
        Destroy();

        alphaValue_ = alphaValue;
        viewPos_.SetPoint(rcPaint.Left, rcPaint.Top);
        viewSize_.SetSize(rcPaint.Width(), rcPaint.Height());
        memDC_ = ::CreateCompatibleDC(hDc);
        bitmap_ = ::CreateCompatibleBitmap(hDc, viewSize_.Width, viewSize_.Height);
        oldBitmap_ = ::SelectObject(memDC_, bitmap_);
        NAssertError(oldBitmap_ != NULL, _T("Error Encountered"));
        ::SetStretchBltMode(memDC_, COLORONCOLOR);

        HRGN rgn = ::CreateRectRgn(0, 0, 1, 1);
        int rgnResult = ::GetClipRgn(hDc, rgn);
        if(rgnResult == 1)
        {
            ::SelectClipRgn(memDC_, rgn);
        }
        ::DeleteObject(rgn);
    }

    void Destroy()
    {
        if(memDC_ != NULL)
        {
            if(oldBitmap_ != NULL)
                ::SelectObject(memDC_, oldBitmap_);
            ::DeleteObject(bitmap_);
            ::DeleteDC(memDC_);
            oldBitmap_ = NULL;
            memDC_ = NULL;
            bitmap_ = NULL;
        }
    }

    const nui::Base::NPoint& GetLeftTop() const
    {
        return viewPos_;
    }

    const nui::Base::NSize& GetSize() const
    {
        return viewSize_;
    }

private:
    HDC memDC_;
    HGDIOBJ oldBitmap_;
    HBITMAP bitmap_;
    BYTE alphaValue_;
    nui::Base::NPoint viewPos_;
    nui::Base::NSize viewSize_;
};