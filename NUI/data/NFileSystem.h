#pragma once


#include "../base/BaseObj.h"
#include "NBuffer.h"

namespace nui
{
    namespace Data
    {
        class NUI_INTF NFileSystem : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("filesystem"))
        public:
            virtual bool Init(LPCTSTR packFilePath) = 0;
            virtual void Destroy() = 0;

            virtual bool IsFileExists(LPCTSTR filePath) = 0;
            virtual bool LoadFile(LPCTSTR filePath, NBuffer* buffer) = 0;
        };
    }
}
