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

            __inline void CheckFlag(DWORD data, DWORD flag, bool check)
            {
                data = check ? (data | flag) : (data & (~flag));
            }
        }
    }
}