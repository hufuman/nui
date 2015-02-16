#pragma once



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
        CGdiHolder(HGDIOBJ obj, bool needDelete)
            : obj_(NULL), needDelete_(false)
        {
            Hold(obj, needDelete);
        }
        CGdiHolder()
            : obj_(NULL), needDelete_(false)
        {}

        ~CGdiHolder()
        {
            Hold(NULL, true);
        }

        operator HGDIOBJ() const
        {
            return obj_;
        }

        void Hold(HGDIOBJ obj, bool needDelete)
        {
            needDelete_ = needDelete;
            obj_ = obj;
        }

        void Destroy()
        {
            if(obj_ && needDelete_)
            {
                ::DeleteObject(obj_);
                obj_ = NULL;
            }
        }
    private:
        HGDIOBJ obj_;
        bool needDelete_;
    };

    class CGdiSelector
    {
        CGdiSelector(const CGdiSelector&);
        CGdiSelector& operator = (const CGdiSelector&);
    public:
        CGdiSelector(HDC dc, HGDIOBJ obj, bool needDelete)
            : dc_(NULL), oldObj_(NULL), needDelete_(false)
        {
            Select(dc, obj, needDelete);
        }
        CGdiSelector()
            : dc_(NULL), oldObj_(NULL), needDelete_(false)
        {}

        ~CGdiSelector()
        {
            Select(NULL, NULL, true);
        }

        void Destroy()
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

        void Select(HDC dc, HGDIOBJ obj, bool needDelete)
        {
            Destroy();
            needDelete_ = needDelete;
            if(dc == NULL || obj == NULL)
                return;

            dc_ = dc;
            oldObj_ = ::SelectObject(dc, obj);
        }
    private:
        HDC dc_;
        HGDIOBJ oldObj_;
        bool needDelete_;
    };
}