#include "stdafx.h"
#include "GdiShape.h"

namespace nui
{
    namespace Ui
    {
        GdiShape::GdiShape()
        {
            style_ = Rect;
            borderAlpha_ = 0;
            fillAlpha_ = 0;
            roundRectWidth_ = 8;
            roundRectHeight_ = 8;
        }
        GdiShape::~GdiShape()
        {
            ;
        }

        NShape& GdiShape::SetStyle(GdiShape::Style style)
        {
            style = style;
            return *this;
        }

        NShape& GdiShape::SetBorder(int borderWidth, ArgbColor borderColor)
        {
            penHolder.Destroy();
            borderAlpha_ = GetAlpha(borderColor);
            if(borderWidth > 0 && borderAlpha_ != 0)
                penHolder.Hold(::CreatePen(PS_SOLID, borderWidth, borderColor & 0x00FFFFFF), true);
            return *this;
        }

        NShape& GdiShape::SetFill(ArgbColor fillColor)
        {
            brushHolder.Destroy();
            fillAlpha_ = GetAlpha(fillColor);
            if(fillAlpha_ != 0)
                brushHolder.Hold(::CreateSolidBrush(fillColor & 0x00FFFFFF), true);
            return *this;
        }

        NShape& GdiShape::SetRoundRectParam(int width, int height)
        {
            roundRectWidth_ = width;
            roundRectHeight_ = height;
            return *this;
        }

        void GdiShape::GetRoundRectParam(int& width, int& height) const
        {
            width = roundRectWidth_;
            height = roundRectHeight_;
        }

        GdiShape::Style GdiShape::GetStyle() const
        {
            return style_;
        }

        BYTE GdiShape::GetBorderAlpha() const
        {
            return borderAlpha_;
        }

        BYTE GdiShape::GetFillAlpha() const
        {
            return fillAlpha_;
        }

        HPEN GdiShape::GetPen() const
        {
            return static_cast<HPEN>((HGDIOBJ)penHolder);
        }

        HBRUSH GdiShape::GetBrush() const
        {
            return static_cast<HBRUSH>((HGDIOBJ)brushHolder);
        }
    }
}
