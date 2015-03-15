#pragma once



#include "../base/BaseObj.h"
#include "../base/DataTypes.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NFont : public Base::NBaseObj
        {
        public:
            virtual NFont& SetFontSize(LONG fontSize) = 0;
            virtual NFont& SetBold(bool bold) = 0;
            virtual NFont& SetItalic(bool italic) = 0;
            virtual NFont& SetUnderline(bool underline) = 0;
            virtual NFont& SetStrikeOut(bool strikeOut) = 0;
            virtual NFont& SetFontName(LPCTSTR fontName) = 0;

            virtual LONG GetFontSize() const= 0;
            virtual bool GetBold() const= 0;
            virtual bool GetItalic() const= 0;
            virtual bool GetUnderline() const= 0;
            virtual bool GetStrikeOut() const= 0;
            virtual Base::NString GetFontName() const= 0;
        };
    }
}
