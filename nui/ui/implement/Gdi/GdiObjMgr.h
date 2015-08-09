#pragma once




namespace nui
{
    namespace Ui
    {
        class GdiObjMgr
        {
            GdiObjMgr();
            GdiObjMgr(const GdiObjMgr&);
            GdiObjMgr& operator = (const GdiObjMgr&);
        public:
            ~GdiObjMgr();

            static GdiObjMgr& Instance();

            void Init();
            void Destroy();

            HFONT GetFont(LONG fontSize, BOOL bold, BOOL italic, BOOL underline, BOOL strikeOut, LPCTSTR fontName);
            void ReleaseFont(HFONT hFont);

            HFONT GetDefaultFont();
            Base::NString GetDefaultFontName() const;

        protected:
            void InitDefaultFont();
            void DestroyFonts();

        protected:
            // Font
            typedef struct
            {
                LOGFONT     LogFont;
                HFONT       hFont;
            } stFontInfo;
            typedef Data::NArrayT<stFontInfo> FontInfoList;
            FontInfoList    fontInfolist_;
            HFONT           defaultFont_;
            Base::NString   defaultFontName_;
        };
    }
}
