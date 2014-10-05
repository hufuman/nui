#include "stdafx.h"

#include "../../Base/NString.h"
#include "../NFileUtil.h"

namespace
{
    bool TestFileRight(LPCTSTR filePath, bool canRead, bool canWrite)
    {
        DWORD desireAccess = (canRead ? GENERIC_READ : 0) | (canWrite ? GENERIC_WRITE : 0);
        HANDLE fileHandle = ::CreateFile(filePath, desireAccess, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
        if(fileHandle == INVALID_HANDLE_VALUE)
            return false;
        ::CloseHandle(fileHandle);
        return true;
    }
}

namespace NUI
{
    namespace Util
    {
        namespace File
        {
            Base::NString GetParentFolder(LPCTSTR path)
            {
                if(path == NULL || path[0] == 0)
                    return Base::NString();

                Base::NString folder(path);
                bool endWithSlash = (folder[folder.GetLength()-1] == _T('\\'));
                size_t pos = endWithSlash ?
                    folder.LastIndexOf(_T('\\'), folder.GetLength()-2)
                    : folder.LastIndexOf(_T('\\'));

                if(pos == -1)
                    return endWithSlash ? path : (folder + _T('\\'));
                return folder.SubString(0, pos + 1);
            }

            bool IsFileExists(LPCTSTR filePath)
            {
                DWORD fileAttr = ::GetFileAttributes(filePath);
                return (fileAttr != INVALID_FILE_ATTRIBUTES
                    && ((fileAttr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY));
            }

            bool IsDirectoryExists(LPCTSTR filePath)
            {
                DWORD fileAttr = ::GetFileAttributes(filePath);
                return (fileAttr != INVALID_FILE_ATTRIBUTES
                    && ((fileAttr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY));
            }

            bool CanWrite(LPCTSTR filePath)
            {
                return TestFileRight(filePath, false, true);
            }

            bool CanRead(LPCTSTR filePath)
            {
                return TestFileRight(filePath, true, false);
            }

            bool CanReadWrite(LPCTSTR filePath)
            {
                return TestFileRight(filePath, true, true);
            }
        }
    }
}