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
            virtual NShape& SetBorder(int borderWidth, ArgbColor borderColor) = 0;
            virtual NShape& SetFill(ArgbColor fillColor) = 0;
            virtual NShape& SetRoundRectParam(int width, int height) = 0;
            virtual void GetRoundRectParam(int& width, int& height) const = 0;
            virtual Style GetStyle() const = 0;
        };
    }
}
