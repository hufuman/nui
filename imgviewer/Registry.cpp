#include "stdafx.h"
#include "Registry.h"


CRegistry::CRegistry()
{
    m_hKey = NULL;
}

CRegistry::~CRegistry()
{
    Close();
}

BOOL CRegistry::OpenForRead(HKEY hKeyRoot, LPCTSTR szSubPath)
{
    Close();

    LSTATUS lResult = ::RegOpenKeyEx(hKeyRoot, szSubPath, 0, KEY_READ, &m_hKey);
    return (lResult == ERROR_SUCCESS);
}

BOOL CRegistry::OpenForWrite(HKEY hKeyRoot, LPCTSTR szSubPath)
{
    Close();

    LSTATUS lResult = ::RegCreateKeyEx(hKeyRoot, szSubPath, 0, NULL, 0, KEY_WRITE, NULL, &m_hKey, NULL);
    return (lResult == ERROR_SUCCESS);
}

void CRegistry::Close()
{
    if(m_hKey != NULL)
    {
        ::RegCloseKey(m_hKey);
        m_hKey = NULL;
    }
}

BOOL CRegistry::GetValue(LPCTSTR szValueName, NString& strValue) const
{
    if(m_hKey == NULL)
        return FALSE;

    TCHAR szBuffer[1024];
    DWORD dwType = REG_NONE;
    DWORD dwLength = 1024 * sizeof(TCHAR) - sizeof(TCHAR);
    LSTATUS lResult = ::RegQueryValueEx(m_hKey, szValueName, 0, &dwType, (LPBYTE)szBuffer, &dwLength);
    if(lResult != ERROR_SUCCESS)
        return FALSE;

    szBuffer[dwLength / sizeof(TCHAR)] = 0;
    strValue = szBuffer;
    return TRUE;
}

BOOL CRegistry::SetValue(LPCTSTR szValueName, const NString& strValue) const
{
    if(m_hKey == NULL)
        return FALSE;

    DWORD dwLength = (strValue.GetLength() + 1) * sizeof(TCHAR);
    LSTATUS lResult = ::RegSetValueEx(m_hKey, szValueName, 0, REG_SZ, (LPBYTE)(LPCTSTR)strValue, dwLength);
    if(lResult != ERROR_SUCCESS)
        return FALSE;

    return TRUE;
}

BOOL CRegistry::GetRegValue(HKEY hKeyRoot, LPCTSTR szSubPath, LPCTSTR szValueName, NString& strValue)
{
    CRegistry registry;
    if(registry.OpenForRead(hKeyRoot, szSubPath)
        && registry.GetValue(szValueName, strValue))
    {
        return TRUE;
    }
    return FALSE;
}

BOOL CRegistry::SetRegValue(HKEY hKeyRoot, LPCTSTR szSubPath, LPCTSTR szValueName, const NString& strValue)
{
    CRegistry registry;
    if(registry.OpenForWrite(hKeyRoot, szSubPath)
        && registry.SetValue(szValueName, strValue))
    {
        return TRUE;
    }
    return FALSE;
}
