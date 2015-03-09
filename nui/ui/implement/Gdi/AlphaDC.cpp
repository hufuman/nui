#include "StdAfx.h"
#include "AlphaDC.h"

#include "SSE.h"

CAlphaDC::CAlphaDC()
{
    memDc_ = NULL;
    memBmp_ = NULL;
    oldBmp_ = NULL;

    orgDc_ = NULL;
}

CAlphaDC::~CAlphaDC(void)
{
    Destroy();
}

bool CAlphaDC::Init(HDC hDc, const nui::Base::NRect& rcPaint, const nui::Base::NSize& size, bool bCopyOrg)
{
    rcPaint_ = rcPaint;

    if(rcPaint_.Left < 0)
        rcPaint_.Left = 0;
    if(rcPaint_.Top < 0)
        rcPaint_.Top = 0;
    if(rcPaint_.Right > size.Width)
        rcPaint_.Right = size.Width;
    if(rcPaint_.Bottom > size.Height)
        rcPaint_.Bottom = size.Height;

    if(rcPaint_.Width() <= 0 || rcPaint_.Height() <= 0)
        return false;

    orgDc_ = hDc;
    if(memBmp_ != NULL && bmpInfo_.bmWidth == rcPaint_.Width() && bmpInfo_.bmHeight == rcPaint_.Height())
    {
        if(bCopyOrg)
            CopyOrg();
        else
            CSSE::MemSetDWord(bmpInfo_.bmBits, 0xFF000000, bmpInfo_.bmWidthBytes * bmpInfo_.bmHeight);
        return true;
    }

    Destroy();

    HDC hScreenDC = ::GetDC(NULL);
    memDc_ = ::CreateCompatibleDC(hScreenDC);

    // Create Bitmap
    BITMAPINFO bi = {0};
    BITMAPINFOHEADER& bih = bi.bmiHeader;

    bih.biSize = sizeof(bih);
    bih.biWidth = rcPaint_.Width();
    bih.biHeight = rcPaint_.Height();
    bih.biPlanes = 1;
    bih.biBitCount = 32;
    bih.biCompression = BI_RGB;
    bih.biSizeImage = 0;
    bih.biXPelsPerMeter = 0;
    bih.biYPelsPerMeter = 0;
    bih.biClrUsed = 0;
    bih.biClrImportant = 0;

    memBmp_ = (HBITMAP)::CreateDIBSection(hScreenDC, &bi, DIB_RGB_COLORS, NULL, 0, 0);
    oldBmp_ = ::SelectObject(memDc_, memBmp_);
    ::ReleaseDC(NULL, hScreenDC);

    ::GetObject(memBmp_, sizeof(bmpInfo_), &bmpInfo_);
    if(bCopyOrg)
    {
        CopyOrg();
    }
    else
    {
        CSSE::MemSetDWord(bmpInfo_.bmBits, 0xFF000000, bmpInfo_.bmWidthBytes * bmpInfo_.bmHeight);
    }

    ::SetViewportOrgEx(memDc_, -rcPaint_.Left, -rcPaint_.Top, NULL);
    return (oldBmp_ != NULL);
}

CAlphaDC::operator HDC ()
{
    return memDc_;
}

bool CAlphaDC::EndDraw(BYTE byAlpha)
{
    CSSE::OpenAlpha(bmpInfo_.bmBits, bmpInfo_.bmWidthBytes * bmpInfo_.bmHeight);

    BLENDFUNCTION BlendFunc = {0};
    BlendFunc.BlendOp = AC_SRC_OVER;
    BlendFunc.SourceConstantAlpha = byAlpha;
    BlendFunc.AlphaFormat = AC_SRC_ALPHA;
    BOOL bResult = ::AlphaBlend(
        orgDc_,
        rcPaint_.Left, rcPaint_.Top,
        rcPaint_.Width(), rcPaint_.Height(),
        memDc_,
        rcPaint_.Left, rcPaint_.Top,
        rcPaint_.Width(), rcPaint_.Height(),
        BlendFunc);
    NAssertError(!!bResult, _T("AlphaBlend Failed"));
    return true;
}

void CAlphaDC::Destroy()
{
    if(memDc_ != NULL)
    {
        if(oldBmp_)
            ::SelectObject(memDc_, oldBmp_);
        if(memBmp_)
            ::DeleteObject(memBmp_);
        if(memDc_)
            ::DeleteDC(memDc_);
        memDc_ = NULL;
        memBmp_ = NULL;
        oldBmp_ = NULL;
    }
}

void CAlphaDC::CopyOrg()
{
    BLENDFUNCTION BlendFunc = {0};
    BlendFunc.BlendOp = AC_SRC_OVER;
    BlendFunc.SourceConstantAlpha = 255;
    BlendFunc.AlphaFormat = AC_SRC_ALPHA;
    BOOL bResult = ::AlphaBlend(
        memDc_,
        0, 0,
        rcPaint_.Width(), rcPaint_.Height(),
        orgDc_,
        rcPaint_.Left, rcPaint_.Top,
        rcPaint_.Width(), rcPaint_.Height(),
        BlendFunc);
    NAssertError(!!bResult, _T("AlphaBlend Failed"));
    ::SetBkMode(memDc_, TRANSPARENT);
    CSSE::DoOr(0xFF000000, bmpInfo_.bmBits, bmpInfo_.bmWidthBytes * bmpInfo_.bmHeight);
}
