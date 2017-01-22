#pragma once



namespace nui
{
    namespace Util
    {
        namespace Misc
        {
            __inline bool IsFlagChecked(DWORD data, DWORD flag)
            {
                return (data & flag) == flag;
            }

            __inline void CheckFlag(DWORD& data, DWORD flag, bool check)
            {
                data = check ? (data | flag) : (data & (~flag));
            }

            __inline Base::NString GetVariantString(LPCTSTR szFormat, ...)
            {
                va_list args;
                va_start(args, szFormat);
                const int nDefaultBufferSize = 1024;
                TCHAR szBuffer[nDefaultBufferSize];
                _vsntprintf(szBuffer, nDefaultBufferSize - 1, szFormat, args);
				szBuffer[nDefaultBufferSize - 1] = 0;
                return szBuffer;
            }
        }
    }
}