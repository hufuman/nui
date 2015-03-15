#pragma once


#include "../../NFont.h"


namespace nui
{
    namespace Ui
    {
        class GdiFont : public NFont
        {
        public:
            GdiFont();
            ~GdiFont();

            enum
            {
                StyleBold       = 0x0001,
                StyleItalic     = 0x0002,
                StyleUnderline  = 0x0004,
                StyleStrikeOut  = 0x0008,
            };

        public:
            virtual NFont& SetFontSize(LONG fontSize);
            virtual NFont& SetBold(bool bold);
            virtual NFont& SetItalic(bool italic);
            virtual NFont& SetUnderline(bool underline);
            virtual NFont& SetStrikeOut(bool strikeOut);
            virtual NFont& SetFontName(LPCTSTR fontName);

            virtual LONG GetFontSize() const;
            virtual bool GetBold() const;
            virtual bool GetItalic() const;
            virtual bool GetUnderline() const;
            virtual bool GetStrikeOut() const;
            virtual Base::NString GetFontName() const;

        public:
            HFONT   GetFont();

        protected:
            void    UpdateFont();
            void    ReleaseFont();

        protected:
            LONG    fontSize_;
            DWORD   fontFlags_;
            Base::NString fontName_;

            HFONT   font_;
        };
    }
}
