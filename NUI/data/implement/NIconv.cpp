#include "stdafx.h"
#include "../NIconv.h"


using namespace NUI;
using namespace Base;

namespace
{

    std::wstring  MultiByteToUnicode(const char* pString, size_t length, UINT uCodePage)
    {
        std::wstring wstrResult;

        int nBufferSize = ::MultiByteToWideChar(uCodePage, 0, pString, length, NULL, 0);
        if(nBufferSize > 0)
        {
            const size_t nStackBufferSize = 4096;
            wchar_t szStackBuffer[nStackBufferSize];
            wchar_t* pBuffer = (nBufferSize >= nStackBufferSize) ? NNewArray<wchar_t>(nBufferSize + 1) : szStackBuffer;
            nBufferSize = ::MultiByteToWideChar(uCodePage, 0, pString, length, pBuffer, nBufferSize);
            if(nBufferSize > 0)
            {
                pBuffer[nBufferSize] = 0;
                wstrResult = pBuffer;
            }
            if(pBuffer != szStackBuffer)
                NFreeArray(pBuffer);
        }
        return wstrResult;
    }

    std::string   UnicodeToMultiByte(const wchar_t* pString, size_t length, UINT uCodePage)
    {
        std::string strResult;

        int nBufferSize = ::WideCharToMultiByte(uCodePage, 0, pString, length, 0, 0, 0, 0);
        if(nBufferSize > 0)
        {
            const size_t nStackBufferSize = 4096;
            char szStackBuffer[nStackBufferSize];
            char* pBuffer = (nBufferSize >= nStackBufferSize) ? NNewArray<char>(nBufferSize + 1) : szStackBuffer;
            nBufferSize = ::WideCharToMultiByte(uCodePage, 0, pString, length, pBuffer, nBufferSize, 0, 0);
            if(nBufferSize > 0)
            {
                pBuffer[nBufferSize] = 0;
                strResult = pBuffer;
            }
            if(pBuffer != szStackBuffer)
                NFreeArray(pBuffer);
        }

        return strResult;
    }
}



namespace NUI
{
    namespace Data
    {
        std::string w2utf8(LPCWSTR arg, size_t length)
        {
            return UnicodeToMultiByte(arg, length, CP_UTF8);
        }

        std::wstring utf82w(LPCSTR arg, size_t length)
        {
            return MultiByteToUnicode(arg, length, CP_UTF8);
        }

        std::string w2a(LPCWSTR arg, size_t length)
        {
            return UnicodeToMultiByte(arg, length, CP_ACP);
        }

        std::wstring a2w(LPCSTR arg, size_t length)
        {
            return MultiByteToUnicode(arg, length, CP_ACP);
        }

        tstring utf82t(LPCSTR arg, size_t length)
        {
#ifdef _UNICODE
            return utf82w(arg, length);
#else
            std::wstring result = utf82w(arg, length);
            return w2a(result.c_str(), result.length());
#endif
        }

        tstring utf82t(LPCSTR arg)
        {
            return utf82t(arg, strlen(arg));
        }

        std::string t2utf8(LPCTSTR arg, size_t length)
        {
#ifdef _UNICODE
            return w2utf8(arg, length);
#else
            std::wstring data = a2w(arg, length);
            return w2utf8(data.c_str(), data.length());
#endif
        }

        std::string t2utf8(LPCTSTR arg)
        {
            return t2utf8(arg, _tcslen(arg));
        }

    }
}