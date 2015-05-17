#pragma once


#include "../../NShapeDraw.h"
#include "../../NRenderDef.h"
#include "GdiUtil.h"

namespace nui
{
    namespace Ui
    {
        class GdiShape : public NShape
        {
        public:
            GdiShape();
            ~GdiShape();

        public:
            virtual NShape* SetStyle(Style style);
            virtual NShape* SetBorderColor(ArgbColor borderColor);
            virtual NShape* SetBorderWidth(int borderWidth);
            virtual NShape* SetFillColor(ArgbColor fillColor);
            virtual Style GetStyle() const;

            BYTE GetBorderAlpha() const;
            ArgbColor GetBorderColor() const;
            int GetBorderWidth() const;
            BYTE GetFillAlpha() const;
            ArgbColor GetFillColor() const;

            // NDraw
            virtual void Draw(NRender* render, int horzIndex, int vertIndex, const Base::NRect& rect);

        private:
            Style style_;
            int borderWidth_;
            ArgbColor borderColor_;
            ArgbColor fillColor_;
        };
    }
}
