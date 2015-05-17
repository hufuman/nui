#include "stdafx.h"
#include "GdiShapeDraw.h"

namespace nui
{
    namespace Ui
    {

        IMPLEMENT_REFLECTION(GdiShapeDraw);

        GdiShapeDraw::GdiShapeDraw()
        {
            style_ = Rect;
            borderWidth_ = 0;
            borderColor_ = 0;
            fillColor_ = 0;
        }
        GdiShapeDraw::~GdiShapeDraw()
        {
            ;
        }

        NShapeDraw* GdiShapeDraw::SetStyle(GdiShapeDraw::Style style)
        {
            style_ = style;
            return this;
        }

        NShapeDraw* GdiShapeDraw::SetBorderColor(ArgbColor borderColor)
        {
            borderColor_ = borderColor;
            return this;
        }

        NShapeDraw* GdiShapeDraw::SetBorderWidth(int borderWidth)
        {
            borderWidth_ = borderWidth;
            return this;
        }

        NShapeDraw* GdiShapeDraw::SetFillColor(ArgbColor fillColor)
        {
            fillColor_ = fillColor;
            return this;
        }

        GdiShapeDraw::Style GdiShapeDraw::GetStyle() const
        {
            return style_;
        }

        BYTE GdiShapeDraw::GetBorderAlpha() const
        {
            return GetAlpha(borderColor_);
        }

        ArgbColor GdiShapeDraw::GetBorderColor() const
        {
            return borderColor_;
        }

        int GdiShapeDraw::GetBorderWidth() const
        {
            return borderWidth_;
        }

        BYTE GdiShapeDraw::GetFillAlpha() const
        {
            return GetAlpha(fillColor_);
        }

        ArgbColor GdiShapeDraw::GetFillColor() const
        {
            return fillColor_;
        }

        void GdiShapeDraw::Draw(NRender* render, int horzIndex, int vertIndex, const Base::NRect& rect)
        {
            UNREFERENCED_PARAMETER(horzIndex);
            UNREFERENCED_PARAMETER(vertIndex);
            render->DrawShape(this, rect);
        }
    }
}
