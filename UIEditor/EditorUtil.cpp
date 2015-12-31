#include "StdAfx.h"
#include "EditorUtil.h"

#pragma warning(disable: 4996)

namespace EditorUtil
{
    bool Execute(LPCTSTR commandLine)
    {
        STARTUPINFO si = {sizeof(STARTUPINFO)};
        PROCESS_INFORMATION pi = {0};
        TCHAR buffer[4096];
        _tcsncpy(buffer, commandLine, _countof(buffer));
        if(!::CreateProcess(NULL, buffer, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
            return false;
        ::CloseHandle(pi.hThread);
        ::CloseHandle(pi.hProcess);
        return true;
    }
};
