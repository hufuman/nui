#pragma once



namespace nui
{
    namespace Util
    {
        namespace File
        {
            NUI_API Base::NString GetParentFolder(LPCTSTR path);
            NUI_API bool IsFileExists(LPCTSTR filePath);
            NUI_API bool IsFolderExists(LPCTSTR filePath);
            NUI_API bool CanWrite(LPCTSTR filePath);
            NUI_API bool CanRead(LPCTSTR filePath);
            NUI_API bool CanReadWrite(LPCTSTR filePath);
            NUI_API Base::NString CombinePath(const Base::NString& path, const Base::NString& pathAfter);
        }
    }
}