#include "stdafx.h"
#include "GdiShape.h"

namespace nui
{
    namespace Ui
    {
        GdiShape::GdiShape()
        {
            style_ = Rect;
            borderWidth_ = 0;
            borderColor_ = 0;
            fillColor_ = 0;
        }
        GdiShape::~GdiShape()
        {
            ;
        }

        NShape* GdiShape::SetStyle(GdiShape::Style style)
        {
            style_ = style;
            return this;
        }

        NShape* GdiShape::SetBorderColor(ArgbColor borderColor)
        {
            borderColor_ = borderColor;
            return this;
        }

        NShape* GdiShape::SetBorderWidth(int borderWidth)
        {
            borderWidth_ = borderWidth;
            return this;
        }

        NShape* GdiShape::SetFillColor(ArgbColor fillColor)
        {
            fillColor_ = fillColor;
            return this;
        }

        GdiShape::Style GdiShape::GetStyle() const
        {
            return style_;
        }

        BYTE GdiShape::GetBorderAlpha() const
        {
            return GetAlpha(borderColor_);
        }

        ArgbColor GdiShape::GetBorderColor() const
        {
            return borderColor_;
        }

        int GdiShape::GetBorderWidth() const
        {
            return borderWidth_;
        }

        BYTE GdiShape::GetFillAlpha() const
        {
            return GetAlpha(fillColor_);
        }

        ArgbColor GdiShape::GetFillColor() const
        {
            return fillColor_;
        }

        void GdiShape::Draw(NRender* render, const Base::NRect& rect)
        {
            render->DrawShape(this, rect);
        }

        bool GdiShape::IsDrawValid() const
        {
            return true;
        }
    }
}
