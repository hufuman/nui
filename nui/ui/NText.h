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
            enum NTextAlign
            {
                TextAlignLeft       = 0x00,
                TextAlignTop        = 0x00,

                TextAlignRight      = 0x01,
                TextAlignBottom     = 0x02,

                TextAlignHCenter    = 0x04,
                TextAlignVCenter    = 0x08,
                TextAlignCenter     = 0x04 | 0x08,
            };

        public:
            virtual NText* SetText(LPCTSTR text) = 0;
            virtual const Base::NString& GetText() const = 0;
            virtual NText* SetColor(ArgbColor color) = 0;
            virtual ArgbColor GetColor() const = 0;
            virtual NText* SetBgColor(ArgbColor color) = 0;
            virtual ArgbColor GetBgColor() const = 0;
            virtual NText* SetSingleLine(bool singleLine) = 0;
            virtual bool GetSingleLine() const = 0;
            virtual NText* SetAlignFlags(UINT alignFlags) = 0;
            virtual UINT GetAlignFlags() const = 0;
        };
    }
}
