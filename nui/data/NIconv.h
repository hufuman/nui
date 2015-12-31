#pragma once


namespace nui
{
    namespace Data
    {
        NUI_API nui::Base::tstring utf82t(LPCSTR arg, size_t length);
        NUI_API nui::Base::tstring utf82t(LPCSTR arg);

        NUI_API std::string t2utf8(LPCTSTR arg, size_t length);
        NUI_API std::string t2utf8(LPCTSTR arg);

        NUI_API std::string a2utf8(LPCSTR arg, size_t length);

        NUI_API nui::Base::tstring a2t(LPCSTR arg);
        NUI_API nui::Base::tstring a2t(LPCSTR arg, size_t length);

    }
}