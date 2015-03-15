#include "stdafx.h"
#include "GdiObjMgr.h"


namespace
{
    LPCTSTR g_FirstDefaultFontName = _T("Î¢ÈíÑÅºÚ");
    LPCTSTR g_SecondDefaultFontName = _T("Tahoma");
    LPCTSTR g_ThirdDefaultFontName = _T("ËÎÌå");

    int CALLBACK EnumDefaultFontNameProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm, DWORD FontType, LPARAM lParam)
    {
        UNREFERENCED_PARAMETER(lpntm);
        UNREFERENCED_PARAMETER(FontType);
        LPCTSTR* fontName = reinterpret_cast<LPCTSTR*>(lParam);

        if(_tcsicmp(lpelf->elfLogFont.lfFaceName, g_FirstDefaultFontName) == 0)
        {
            *fontName = g_FirstDefaultFontName;
        }
        else if(_tcsicmp(lpelf->elfLogFont.lfFaceName, g_SecondDefaultFontName) == 0)
        {
            *fontName = g_SecondDefaultFontName;
        }
        else if(_tcsicmp(lpelf->elfLogFont.lfFaceName, g_ThirdDefaultFontName) == 0)
        {
            *fontName = g_ThirdDefaultFontName;
        }
        return *fontName == NULL;
    }
}
namespace nui
{
    namespace Ui
    {
        GdiObjMgr::GdiObjMgr()
        {
            ;
        }

        GdiObjMgr::~GdiObjMgr()
        {
            ;
        }

        GdiObjMgr& GdiObjMgr::Instance()
        {
            static GdiObjMgr inst;
            return inst;
        }

        void GdiObjMgr::Init()
        {
            InitDefaultFont();
        }

        void GdiObjMgr::Destroy()
        {
            DestroyFonts();
        }

        void GdiObjMgr::DestroyFonts()
        {
            int count = fontInfolist_.Count();
            for(int i=0; i<count; ++ i)
            {
                stFontInfo& info = fontInfolist_[i];
                ::DeleteObject(info.hFont);
            }
            fontInfolist_.RemoveAllItems();
            if(defaultFont_ != NULL)
            {
                ::DeleteObject(defaultFont_);
                defaultFont_ = NULL;
            }
        }

        HFONT GdiObjMgr::GetFont(LONG lFontSize, BOOL bBold, BOOL bItalic, BOOL bUnderline, BOOL bStrikeOut, LPCTSTR szFontName)
        {
            bItalic = !!bItalic;
            bUnderline = !!bUnderline;
            bStrikeOut = !!bStrikeOut;

            LONG lHeight = -lFontSize;
            LONG lWeight = bBold ? FW_BOLD : FW_NORMAL;

            Base::NString strFontName = (szFontName == NULL) ? defaultFontName_.GetData() : szFontName;
            strFontName.MakeLower();

            int index = -1;
            int count = fontInfolist_.Count();
            for(int i=0; i<count; ++ i)
            {
                stFontInfo& info = fontInfolist_[i];
                LOGFONT& LogFont = info.LogFont;
                if(lHeight == LogFont.lfHeight
                    && lWeight == LogFont.lfWeight
                    && bItalic == LogFont.lfItalic
                    && bUnderline == LogFont.lfUnderline
                    && bStrikeOut == LogFont.lfStrikeOut
                    && strFontName == LogFont.lfFaceName)
                {
                    break;
                }
            }

            if(index >= 0)
                return fontInfolist_[index].hFont;

            // Create Font
            stFontInfo info = {0};

            info.LogFont.lfHeight = lHeight;
            info.LogFont.lfWidth = 0;
            info.LogFont.lfEscapement = 0;
            info.LogFont.lfOrientation = 0;
            info.LogFont.lfWeight = lWeight;
            info.LogFont.lfItalic = bItalic ? 1 : 0;
            info.LogFont.lfUnderline = bUnderline ? 1 : 0;
            info.LogFont.lfStrikeOut = bStrikeOut ? 1 : 0;
            info.LogFont.lfCharSet = GB2312_CHARSET; // TODO
            info.LogFont.lfOutPrecision = OUT_TT_PRECIS;
            info.LogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
            info.LogFont.lfQuality = CLEARTYPE_QUALITY;
            info.LogFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
            _tcsncpy(info.LogFont.lfFaceName, strFontName.GetData(), LF_FACESIZE - 1);

            info.hFont = CreateFontIndirect(&info.LogFont);
            NAssertError(info.hFont != NULL, _T("CreateFont Failed"));
            if(info.hFont == NULL)
                return NULL;

            fontInfolist_.AddItem(info);
            return info.hFont;
        }

        void GdiObjMgr::ReleaseFont(HFONT hFont)
        {
            UNREFERENCED_PARAMETER(hFont);
            // No need to release
        }

        Base::NString GdiObjMgr::GetDefaultFontName() const
        {
            return defaultFontName_;
        }

        void GdiObjMgr::InitDefaultFont()
        {
            HDC hDc = ::GetDC(NULL);
            if(hDc == NULL)
            {
                defaultFontName_ = _T("Arial");
            }
            else
            {
                LPCTSTR fontName = NULL;
                EnumFontFamilies(hDc, NULL, (FONTENUMPROC)&EnumDefaultFontNameProc, (LPARAM)&fontName);
                if(fontName == NULL)
                    defaultFontName_ = _T("Arial");
                else
                    defaultFontName_ = fontName;
            }
            ::ReleaseDC(NULL, hDc);
        }
    }
}
