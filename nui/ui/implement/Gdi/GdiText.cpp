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
            horzCenter_ = false;
            vertCenter_ = false;
        }

        GdiText::~GdiText()
        {
            ;
        }

        NText& GdiText::SetText(LPCTSTR text)
        {
            text_ = text;
            return *this;
        }

        Base::NString GdiText::GetText() const
        {
            return text_;
        }

        NText& GdiText::SetColor(ArgbColor color)
        {
            color_ = color;
            return *this;
        }

        ArgbColor GdiText::GetColor() const
        {
            return color_;
        }

        NText& GdiText::SetBgColor(ArgbColor bgColor)
        {
            bgColor_ = bgColor;
            return *this;
        }

        ArgbColor GdiText::GetBgColor() const
        {
            return bgColor_;
        }

        NText& GdiText::SetSingleLine(bool singleLine)
        {
            singleLine_ = singleLine;
            return *this;
        }

        bool GdiText::GetSingleLine() const
        {
            return singleLine_;
        }

        NText& GdiText::SetHorzCenter(bool center)
        {
            horzCenter_ = center;
            return *this;
        }

        bool GdiText::GetHorzCenter() const
        {
            return horzCenter_;
        }

        NText& GdiText::SetVertCenter(bool center)
        {
            vertCenter_ = center;
            return *this;
        }

        bool GdiText::GetVertCenter() const
        {
            return vertCenter_;
        }

        DWORD GdiText::GetDrawFlags() const
        {
            DWORD result = 0;
            result |= (singleLine_ ? DT_SINGLELINE : (DT_EDITCONTROL | DT_WORDBREAK));
            result |= (horzCenter_ ? DT_CENTER : 0);
            result |= (singleLine_ && vertCenter_ ? DT_VCENTER : 0);
            return result;
        }
    }
}
