#pragma once


#include "../../NText.h"


namespace nui
{
    namespace Ui
    {
        class GdiText : public NText
        {
        public:
            GdiText();
            ~GdiText();

        public:
            virtual NText* SetText(LPCTSTR text);
            virtual const Base::NString& GetText() const;
            virtual NText* SetColor(ArgbColor color);
            virtual ArgbColor GetColor() const;
            virtual NText* SetBgColor(ArgbColor color);
            virtual ArgbColor GetBgColor() const;
            virtual NText* SetSingleLine(bool singleLine);
            virtual bool GetSingleLine() const;
            virtual NText* SetAlignFlags(UINT alignFlags);
            virtual UINT GetAlignFlags() const;

        public:
            DWORD GetDrawFlags() const;
            bool IsHorzAlign() const;
            bool IsVertAlign() const;

        private:
            ArgbColor color_;
            ArgbColor bgColor_;
            Base::NString text_;
            bool singleLine_;
            UINT alignFlags_;
        };
    }
}
