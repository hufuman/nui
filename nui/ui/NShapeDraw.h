#pragma once



#include "./NDraw.h"
#include "../base/DataTypes.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NShapeDraw : public NDraw
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("shape"))

        public:
            enum Style
            {
                Rect,
                RoundRect,
                Line,
            };

        public:
            virtual NShapeDraw* SetStyle(Style style) = 0;
            virtual NShapeDraw* SetBorderColor(ArgbColor borderColor) = 0;
            virtual NShapeDraw* SetBorderWidth(int borderWidth) = 0;
            virtual NShapeDraw* SetFillColor(ArgbColor fillColor) = 0;

            virtual Style GetStyle() const = 0;
        };

        NUI_API Base::NAutoPtr<NShapeDraw> CreateRectDraw(ArgbColor fillColor);
    }
}
