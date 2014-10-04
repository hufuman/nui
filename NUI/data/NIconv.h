#pragma once


namespace NUI
{
    namespace Data
    {
        NUI_API NUI::Base::tstring utf82t(LPCSTR arg, size_t length);
        NUI_API NUI::Base::tstring utf82t(LPCSTR arg);

        NUI_API std::string t2utf8(LPCTSTR arg, size_t length);
        NUI_API std::string t2utf8(LPCTSTR arg);

    }
}