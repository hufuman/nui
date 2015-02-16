#include "stdafx.h"
#include "../NBrush.h"


namespace nui
{
    namespace Ui
    {
        NBrush::NBrush(Style style, ArgbColor clrBack, ArgbColor clrFront, NImage* pattern)
            : style_(style), clrBack_(clrBack), clrFront_(clrFront), pattern_(pattern)
        {}

        NBrush::NBrush()
        {
            style_ = Solid;
            clrBack_ = MakeArgb(0, 0, 0, 0);
            clrFront_ = MakeArgb(0xFF, 0xFF, 0xFF, 0xFF);
            pattern_ = NULL;
        }

        NBrush& NBrush::SetStyle(NBrush::Style style)
        {
            style_ = style;
            return *this;
        }

        NBrush& NBrush::SetFrontColor(ArgbColor color)
        {
            clrFront_ = color;
            return *this;
        }

        NBrush& NBrush::SetBackColor(ArgbColor color)
        {
            clrBack_ = color;
            return *this;
        }

        NBrush& NBrush::SetPattern(NImage* pattern)
        {
            pattern_ = pattern;
            return *this;
        }

        NBrush::Style NBrush::GetStyle() const
        {
            return style_;
        }

        ArgbColor NBrush::GetFrontColor() const
        {
            return clrFront_;
        }

        ArgbColor NBrush::GetBackColor() const
        {
            return clrBack_;
        }

        NImage* NBrush::GetPattern() const
        {
            return pattern_;
        }

        NBrush NBrush::ExistedBrush(NBrush::Existed, MakeArgb(0, 0, 0, 0), MakeArgb(0xFF, 0xFF, 0xFF, 0xFF), NULL);
        NBrush NBrush::TransparentBrush(NBrush::Solid, MakeArgb(0, 0, 0, 0), MakeArgb(0xFF, 0xFF, 0xFF, 0xFF), NULL);
    }
}
