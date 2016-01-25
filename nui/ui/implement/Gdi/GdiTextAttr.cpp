#include "stdafx.h"
#include "GdiTextAttr.h"

#include "GdiResourceLoader.h"

namespace nui
{
    namespace Ui
    {
        GdiTextAttr::GdiTextAttr()
        {
            color_ = MakeArgb(255, 0, 0, 0);
            bgColor_ = MakeArgb(0, 0, 0, 0);
            singleLine_ = true;
            alignFlags_ = TextAlignCenter;
        }

        GdiTextAttr::~GdiTextAttr()
        {
            ;
        }

        NTextAttr* GdiTextAttr::SetColor(ArgbColor color)
        {
            color_ = color;
            return this;
        }

        ArgbColor GdiTextAttr::GetColor() const
        {
            return color_;
        }

        NTextAttr* GdiTextAttr::SetBgColor(ArgbColor bgColor)
        {
            bgColor_ = bgColor;
            return this;
        }

        ArgbColor GdiTextAttr::GetBgColor() const
        {
            return bgColor_;
        }

        NTextAttr* GdiTextAttr::SetSingleLine(bool singleLine)
        {
            singleLine_ = singleLine;
            return this;
        }

        bool GdiTextAttr::IsSingleLine() const
        {
            return singleLine_;
        }

        NTextAttr* GdiTextAttr::SetAlignFlags(UINT alignFlags)
        {
            alignFlags_ = alignFlags;
            return this;
        }

        UINT GdiTextAttr::GetAlignFlags() const
        {
            return alignFlags_;
        }

        DWORD GdiTextAttr::GetDrawFlags() const
        {
            DWORD result = 0;
            result |= (singleLine_ ? (DT_SINGLELINE | DT_END_ELLIPSIS) : (DT_EDITCONTROL | DT_WORDBREAK));

            if(alignFlags_ & TextAlignHCenter)
                result |= DT_CENTER;
            else if(alignFlags_ & TextAlignRight)
                result |= DT_RIGHT;
            else
                result |= DT_LEFT;

            if((alignFlags_ & TextAlignVCenter) && singleLine_)
                result |= DT_VCENTER;
            else if(alignFlags_ & TextAlignBottom)
                result |= DT_BOTTOM;
            else
                result |= DT_TOP;

            return result;
        }

        bool GdiTextAttr::IsHorzAlign() const
        {
            return (alignFlags_ & TextAlignHCenter) == TextAlignHCenter;
        }

        bool GdiTextAttr::IsVertAlign() const
        {
            return (alignFlags_ & TextAlignVCenter) == TextAlignVCenter;
        }
    }
}
