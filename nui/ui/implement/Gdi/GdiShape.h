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
            virtual NShape& SetBorder(int borderWidth, ArgbColor borderColor);
            virtual NShape& SetFill(ArgbColor fillColor);
            virtual Style GetStyle() const;
            virtual NShape& SetRoundRectParam(int width, int height);
            virtual void GetRoundRectParam(int& width, int& height) const;

            BYTE GetBorderAlpha() const;
            BYTE GetFillAlpha() const;

            HPEN GetPen() const;
            HBRUSH GetBrush() const;

        private:
            Style style_;
            BYTE borderAlpha_;
            BYTE fillAlpha_;
            int roundRectWidth_;
            int roundRectHeight_;
            Gdi::CGdiHolder penHolder;
            Gdi::CGdiHolder brushHolder;
        };
    }
}
