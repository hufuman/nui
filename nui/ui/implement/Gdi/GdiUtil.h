#pragma once

#include "../../NImageDraw.h"

namespace Gdi
{
    __inline bool IsColorTransparent(nui::Ui::ArgbColor color)
    {
        return (color & 0xFF000000) == 0x00000000;
    }

    __inline bool IsColorOpaque(nui::Ui::ArgbColor color)
    {
        return (color & 0xFF000000) == 0xFF000000;
    }

    class CGdiHolder
    {
        CGdiHolder(const CGdiHolder&);
        CGdiHolder& operator = (const CGdiHolder&);
    public:
        CGdiHolder(HGDIOBJ obj, bool needDelete);
        CGdiHolder();
        ~CGdiHolder();

        operator HGDIOBJ() const;
        void Hold(HGDIOBJ obj, bool needDelete);
        void Destroy();
    private:
        HGDIOBJ obj_;
        bool needDelete_;
    };

    class CGdiSelector
    {
        CGdiSelector(const CGdiSelector&);
        CGdiSelector& operator = (const CGdiSelector&);
    public:
        CGdiSelector(HDC dc, HGDIOBJ obj, bool needDelete);
        CGdiSelector();
        ~CGdiSelector();

        void Destroy();
        void Select(HDC dc, HGDIOBJ obj, bool needDelete);
    private:
        HDC dc_;
        HGDIOBJ oldObj_;
        bool needDelete_;
    };

    bool SaveBitmap(HBITMAP bitmap, LPCTSTR filePath);

    bool GetImageData(const BYTE* pData, DWORD dwSize, nui::Ui::stImageExtInfo& extInfo);
    bool SetImageData(const nui::Ui::stImageExtInfo& extInfo, std::string& strData);
}
