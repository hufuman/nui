#pragma once


#include "../base/BaseObj.h"

namespace nui
{
    namespace Data
    {
        class NUI_INTF NZip : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("zip"))
        public:
            // Zip
            virtual bool ZipFolder(LPCTSTR srcFolder, LPCTSTR outputFile) = 0;

            // UnZip
            virtual bool LoadFile(LPCTSTR zipFilePath) = 0;
            virtual bool GetFileContent(LPCTSTR relativePath, NBuffer* buffer) = 0;
            virtual bool IsFileExists(LPCTSTR relativePath) = 0;
            virtual void Close() = 0;
        };
    }
}
