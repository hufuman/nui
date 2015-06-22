#pragma once


class NTempDC
{
    NTempDC(const MemDC&);
    NTempDC& operator = (const MemDC&);
public:
    NTempDC()
    {
        memDC_ = NULL;
        bitmap_ = NULL;
        oldBitmap_ = NULL;
    }
    ~NTempDC()
    {
        Destroy();
    }

    operator HDC()
    {
        return memDC_;
    }

    HDC Init()
    {
        Destroy();

        HDC hScreenDc = ::GetDC(NULL);
        memDC_ = ::CreateCompatibleDC(hScreenDc);
        bitmap_ = ::CreateCompatibleBitmap(hScreenDc, 1, 1);
        oldBitmap_ = ::SelectObject(memDC_, bitmap_);
        ::ReleaseDC(NULL, hScreenDc);
        return memDC_;
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

private:
    HDC memDC_;
    HGDIOBJ oldBitmap_;
    HBITMAP bitmap_;
};
