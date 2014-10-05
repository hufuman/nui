#pragma once


#include "../base/BaseObj.h"

namespace NUI
{
    namespace Data
    {
        class NUI_INTF NBuffer : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("buffer"))
        public:
            virtual NBuffer& SetSize(DWORD size) = 0;
            virtual DWORD GetSize() const = 0;
            virtual LPVOID GetBuffer() = 0;
            virtual LPVOID GetBuffer(DWORD size) = 0;
            virtual void ReleaseBuffer() = 0;
        };
    }
}
