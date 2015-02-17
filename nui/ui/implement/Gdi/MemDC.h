#pragma once


#include "../../../Base/DataTypes.h"


class MemDC
{
    MemDC(const MemDC&);
    MemDC& operator = (const MemDC&);
public:
    MemDC()
    {
        dc_ = NULL;
        memDC_ = NULL;
        oldBitmap_ = NULL;
        bitmap_ = NULL;
    }
    ~MemDC()
    {
        DrawBack();
        Destroy();
    }

    operator HDC()
    {
        return dc_;
    }

    void Init(HDC hDc, const nui::Base::NRect& rcPaint, BYTE alphaValue)
    {
        Destroy();

        dc_ = hDc;
        alphaValue_ = alphaValue;
        viewport_ = rcPaint;
        memDC_ = ::CreateCompatibleDC(dc_);
        bitmap_ = ::CreateCompatibleBitmap(dc_, viewport_.Width(), viewport_.Height());
        oldBitmap_ = ::SelectObject(memDC_, bitmap_);
        NAssertError(oldBitmap_ != NULL, _T("Error Encountered"));
        ::SetStretchBltMode(memDC_, COLORONCOLOR);

        HRGN rgn = ::CreateRectRgn(0, 0, 1, 1);
        int rgnResult = ::GetClipRgn(hDc, rgn);
        if(rgnResult == 1)
        {
            ::SelectClipRgn(memDC_, rgn);
        }
        else
        {
            ::DeleteObject(rgn);
        }
    }

    void DrawBack()
    {
        if(oldBitmap_ == NULL)
            return;

        BLENDFUNCTION BlendFunc = {0};
        BlendFunc.BlendOp = AC_SRC_OVER;
        BlendFunc.SourceConstantAlpha = alphaValue_;
        BlendFunc.AlphaFormat = AC_SRC_ALPHA;
        BOOL bResult = ::AlphaBlend(dc_,
            viewport_.Left, viewport_.Top,
            viewport_.Width(), viewport_.Height(),
            memDC_,
            0, 0,
            viewport_.Width(), viewport_.Height(),
            BlendFunc);
        NAssertError(!!bResult, _T("AlphaBlend Failed"));
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
            dc_  = NULL;
            viewport_.SetRect(0, 0, 0, 0);
        }
    }

private:
    HDC dc_;
    HDC memDC_;
    HGDIOBJ oldBitmap_;
    HBITMAP bitmap_;
    BYTE alphaValue_;
    nui::Base::NRect viewport_;
};