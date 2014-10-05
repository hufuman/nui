#pragma once


#include "../base/BaseObj.h"

namespace NUI
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
            virtual bool GetFileContent(LPCTSTR relativePath, int& index, LPBYTE& data, DWORD& size) = 0;
            virtual void ReleaseFileContent(int index) = 0;
            virtual void Close() = 0;
        };
    }
}
