#pragma once


#include "../../NTextAttr.h"


namespace nui
{
    namespace Ui
    {
#define GDI_TEXTATTR_DEFAULT_DRAWFLAGS (DT_SINGLELINE | DT_END_ELLIPSIS | DT_CENTER | DT_VCENTER)

        class GdiTextAttr : public NTextAttr
        {
        public:
            GdiTextAttr();
            ~GdiTextAttr();

        public:
            virtual NTextAttr* SetColor(ArgbColor color);
            virtual ArgbColor GetColor() const;
            virtual NTextAttr* SetBgColor(ArgbColor color);
            virtual ArgbColor GetBgColor() const;
            virtual NTextAttr* SetSingleLine(bool singleLine);
            virtual bool IsSingleLine() const;
            virtual NTextAttr* SetAlignFlags(UINT alignFlags);
            virtual UINT GetAlignFlags() const;

        public:
            DWORD GetDrawFlags() const;
            bool IsHorzAlign() const;
            bool IsVertAlign() const;

        private:
            ArgbColor color_;
            ArgbColor bgColor_;
            bool singleLine_;
            UINT alignFlags_;
        };
    }
}
