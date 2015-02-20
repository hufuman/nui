#pragma once

#include "../base/BaseObj.h"
#include "../base/DataTypes.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NShape : public Base::NBaseObj
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
            virtual NShape& SetStyle(Style style) = 0;
            virtual NShape& SetBorderColor(ArgbColor borderColor) = 0;
            virtual NShape& SetBorderWidth(int borderWidth) = 0;
            virtual NShape& SetFillColor(ArgbColor fillColor) = 0;

            virtual Style GetStyle() const = 0;
        };
    }
}
