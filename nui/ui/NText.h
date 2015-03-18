#pragma once



#include "../base/BaseObj.h"
#include "../base/DataTypes.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NText : public Base::NBaseObj
        {
        public:
            virtual NText& SetText(LPCTSTR text) = 0;
            virtual Base::NString GetText() const = 0;
            virtual NText& SetColor(ArgbColor color) = 0;
            virtual ArgbColor GetColor() const = 0;
            virtual NText& SetBgColor(ArgbColor color) = 0;
            virtual ArgbColor GetBgColor() const = 0;
            virtual NText& SetSingleLine(bool singleLine) = 0;
            virtual bool GetSingleLine() const = 0;
            virtual NText& SetHorzCenter(bool center) = 0;
            virtual bool GetHorzCenter() const = 0;
            virtual NText& SetVertCenter(bool center) = 0;
            virtual bool GetVertCenter() const = 0;
        };
    }
}
