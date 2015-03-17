#pragma once


// See also:
// http://www.codeproject.com/Articles/16796/Riding-the-Vista-UAC-elevator-up-and-down


namespace ProcUtil
{
    BOOL IsElevated();
    BOOL RunNonElevated(LPCTSTR szExePath, LPCTSTR szArgs, LPCTSTR szDirectory);
    BOOL RunElevated(LPCTSTR szExePath, LPCTSTR szArgs, LPCTSTR szDirectory);

    BOOL RunNonElevated(LPCTSTR szCmdLine, LPCTSTR szDirectory);
    BOOL RunElevated(LPCTSTR szCmdLine, LPCTSTR szDirectory);

    NString CombineCmdLine(LPCTSTR szExePath, LPCTSTR szArgs);
};
