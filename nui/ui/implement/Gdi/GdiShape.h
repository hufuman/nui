#pragma once


#include "../../NShape.h"
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
            virtual NShape& SetStyle(Style style);
            virtual NShape& SetBorderColor(ArgbColor borderColor);
            virtual NShape& SetBorderWidth(int borderWidth);
            virtual NShape& SetFillColor(ArgbColor fillColor);
            virtual Style GetStyle() const;

            BYTE GetBorderAlpha() const;
            ArgbColor GetBorderColor() const;
            int GetBorderWidth() const;
            BYTE GetFillAlpha() const;
            ArgbColor GetFillColor() const;

        private:
            Style style_;
            int borderWidth_;
            ArgbColor borderColor_;
            ArgbColor fillColor_;
        };
    }
}
