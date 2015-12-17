#include "stdafx.h"
#include "GdiText.h"

#include "GdiResourceLoader.h"

namespace nui
{
    namespace Ui
    {
        GdiText::GdiText()
        {
            color_ = MakeArgb(255, 0, 0, 0);
            bgColor_ = MakeArgb(0, 0, 0, 0);
            text_ = _T("");
            singleLine_ = true;
            alignFlags_ = TextAlignCenter;
        }

        GdiText::~GdiText()
        {
            ;
        }

        NText* GdiText::SetText(LPCTSTR text)
        {
            text_ = text;
            return this;
        }

        const Base::NString& GdiText::GetText() const
        {
            return text_;
        }

        NText* GdiText::SetColor(ArgbColor color)
        {
            color_ = color;
            return this;
        }

        ArgbColor GdiText::GetColor() const
        {
            return color_;
        }

        NText* GdiText::SetBgColor(ArgbColor bgColor)
        {
            bgColor_ = bgColor;
            return this;
        }

        ArgbColor GdiText::GetBgColor() const
        {
            return bgColor_;
        }

        NText* GdiText::SetSingleLine(bool singleLine)
        {
            singleLine_ = singleLine;
            return this;
        }

        bool GdiText::GetSingleLine() const
        {
            return singleLine_;
        }

        NText* GdiText::SetAlignFlags(UINT alignFlags)
        {
            alignFlags_ = alignFlags;
            return this;
        }

        UINT GdiText::GetAlignFlags() const
        {
            return alignFlags_;
        }

        DWORD GdiText::GetDrawFlags() const
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

        bool GdiText::IsHorzAlign() const
        {
            return (alignFlags_ & TextAlignHCenter) == TextAlignHCenter;
        }

        bool GdiText::IsVertAlign() const
        {
            return (alignFlags_ & TextAlignVCenter) == TextAlignVCenter;
        }
    }
}
