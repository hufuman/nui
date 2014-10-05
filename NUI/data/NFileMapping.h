#pragma once


#include "../base/BaseObj.h"

namespace NUI
{
    namespace Data
    {
        class NUI_INTF NFileMapping : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("filemapping"))
        public:
            virtual bool OpenForRead(LPCTSTR filePath) = 0;
            virtual void Close() = 0;
            virtual LPBYTE GetData() = 0;
            virtual DWORD GetSize() = 0;
        };
    }
}
