#include "stdafx.h"
#include "GdiFont.h"

#include "GdiObjMgr.h"

namespace
{
    const int g_DefaultFontSize = 12;
}

namespace nui
{
    namespace Ui
    {
        GdiFont::GdiFont()
        {
            font_ = NULL;
            fontSize_ = g_DefaultFontSize;
            fontFlags_ = 0;
        }

        GdiFont::~GdiFont()
        {
            ;
        }

        //////////////////////////////////////////////////////////////////////////
        // IXFont
        NFont* GdiFont::SetFontSize(LONG fontSize)
        {
            if(fontSize_ != fontSize)
            {
                fontSize_ = fontSize;
                UpdateFont();
            }
            return this;
        }

        NFont* GdiFont::SetBold(bool bold)
        {
            bool oldBold = (fontFlags_ & StyleBold) == StyleBold;
            if(oldBold != bold)
            {
                fontFlags_ = fontFlags_ | StyleBold;
                UpdateFont();
            }
            return this;
        }

        NFont* GdiFont::SetItalic(bool italic)
        {
            bool oldItalic = (fontFlags_ & StyleItalic) == StyleItalic;
            if(oldItalic != italic)
            {
                fontFlags_ = fontFlags_ | StyleItalic;
                UpdateFont();
            }
            return this;
        }

        NFont* GdiFont::SetUnderline(bool underline)
        {
            bool oldUnderline = (fontFlags_ & StyleUnderline) == StyleUnderline;
            if(oldUnderline != underline)
            {
                fontFlags_ = fontFlags_ | StyleUnderline;
                UpdateFont();
            }
            return this;
        }

        NFont* GdiFont::SetStrikeOut(bool strikeOut)
        {
            bool oldStrikeOut = (fontFlags_ & StyleStrikeOut) == StyleStrikeOut;
            if(oldStrikeOut != strikeOut)
            {
                fontFlags_ = fontFlags_ | StyleStrikeOut;
                UpdateFont();
            }
            return this;
        }

        NFont* GdiFont::SetFontName(LPCTSTR fontName)
        {
            if(fontName_ != fontName)
            {
                fontName_ = fontName;
                UpdateFont();
            }
            return this;
        }

        LONG GdiFont::GetFontSize() const
        {
            return fontSize_;
        }

        bool GdiFont::GetBold() const
        {
            return (fontFlags_ & StyleBold) == StyleBold;
        }

        bool GdiFont::GetItalic() const
        {
            return (fontFlags_ & StyleItalic) == StyleItalic;
        }

        bool GdiFont::GetUnderline() const
        {
            return (fontFlags_ & StyleUnderline) == StyleUnderline;
        }

        bool GdiFont::GetStrikeOut() const
        {
            return (fontFlags_ & StyleStrikeOut) == StyleStrikeOut;
        }

        const Base::NString& GdiFont::GetFontName() const
        {
            return fontName_;
        }

        void GdiFont::UpdateFont()
        {
            ReleaseFont();
        }

        HFONT GdiFont::GetFont()
        {
            if(font_ != NULL)
                return font_;

            if(fontSize_ == g_DefaultFontSize && fontFlags_ == 0 && fontName_.IsEmpty())
                return NULL;

            GdiObjMgr& gdiObjMgr = GdiObjMgr::Instance();
            font_ = gdiObjMgr.GetFont(fontSize_,
                (fontFlags_ & StyleBold) == StyleBold,
                (fontFlags_ & StyleItalic) == StyleItalic,
                (fontFlags_ & StyleUnderline) == StyleUnderline,
                (fontFlags_ & StyleStrikeOut) == StyleStrikeOut,
                fontName_.IsEmpty() ? GdiObjMgr::Instance().GetDefaultFontName() : fontName_);
            NAssertError(font_ != NULL, _T("CreateFont Failed"));

            return font_;
        }

        void GdiFont::ReleaseFont()
        {
            if(font_ != NULL)
            {
                GdiObjMgr& gdiObjMgr = GdiObjMgr::Instance();
                gdiObjMgr.ReleaseFont(font_);
                font_ = NULL;
            }
        }
    }
}