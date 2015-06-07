#pragma once


#include "../../NShapeDraw.h"
#include "../../NRenderDef.h"
#include "GdiUtil.h"

namespace nui
{
    namespace Ui
    {
        class GdiShapeDraw : public NShapeDraw
        {
        public:
            GdiShapeDraw();
            ~GdiShapeDraw();

        public:
            virtual Base::NSize GetPreferSize() const;

            virtual NShapeDraw* SetStyle(Style style);
            virtual NShapeDraw* SetBorderColor(ArgbColor borderColor);
            virtual NShapeDraw* SetBorderWidth(int borderWidth);
            virtual NShapeDraw* SetFillColor(ArgbColor fillColor);
            virtual Style GetStyle() const;

            BYTE GetBorderAlpha() const;
            ArgbColor GetBorderColor() const;
            int GetBorderWidth() const;
            BYTE GetFillAlpha() const;
            ArgbColor GetFillColor() const;

            // NDraw
            virtual NDraw* Draw(NRender* render, int horzIndex, int vertIndex, const Base::NRect& rect);

        private:
            Style style_;
            int borderWidth_;
            ArgbColor borderColor_;
            ArgbColor fillColor_;
        };
    }
}
