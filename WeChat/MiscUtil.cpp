#include "StdAfx.h"
#include "MiscUtil.h"



namespace
{
    typedef struct
    {
        ULONG i[2];
        ULONG buf[4];
        unsigned char in[64];
        unsigned char digest[16];
    } MD5_CTX;
    typedef void (__stdcall *PFN_MD5Init)(MD5_CTX* context);
    typedef void (__stdcall *PFN_MD5Update)(MD5_CTX* context, unsigned char* input, unsigned int inlen);
    typedef void (__stdcall *PFN_MD5Final)(MD5_CTX* context);

    bool g_CryptModuleLoaded = false;
    HMODULE g_CryptModule = NULL;
    PFN_MD5Init g_pfnMD5Init = NULL;
    PFN_MD5Update g_pfnMD5Update = NULL;
    PFN_MD5Final g_pfnMD5Final = NULL;

    bool EnsureMd5Funcs()
    {
        if(!g_CryptModuleLoaded)
        {
            g_CryptModuleLoaded = true;
            g_CryptModule = ::LoadLibrary(_T("Cryptdll.dll"));
            if(g_CryptModule == NULL)
                return false;
            g_pfnMD5Init = (PFN_MD5Init)::GetProcAddress(g_CryptModule, "MD5Init");
            g_pfnMD5Update = (PFN_MD5Update)::GetProcAddress(g_CryptModule, "MD5Update");
            g_pfnMD5Final = (PFN_MD5Final)::GetProcAddress(g_CryptModule, "MD5Final");
        }
        return g_pfnMD5Init != NULL && g_pfnMD5Update != NULL && g_pfnMD5Final != NULL;
    }

    TCHAR g_BaseMap[] = _T("0123456789ABCDEF");

    NString ToHex(BYTE* data, int length)
    {
        NString result;
        for(int i=0; i<length; ++ i)
        {
            result += g_BaseMap[((data[i] & 0xF0) >> 4) & 0x0F];
            result += g_BaseMap[data[i] & 0x0F];
        }
        return result;
    }
}

namespace MiscUtil
{
    NString GetMd5(unsigned char* data, int length)
    {
        if(!EnsureMd5Funcs())
            return _T("");
        MD5_CTX context;
        g_pfnMD5Init(&context);
        g_pfnMD5Update(&context, reinterpret_cast<unsigned char*>(data), length);
        g_pfnMD5Final(&context);
        return ToHex(context.digest, _countof(context.digest));
    }
}
