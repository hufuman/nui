#pragma once

class CRegistry
{
    CRegistry(const CRegistry&);
    CRegistry& operator = (const CRegistry&);
public:
    CRegistry();
    ~CRegistry();

    BOOL OpenForRead(HKEY hKeyRoot, LPCTSTR szSubPath);
    BOOL OpenForWrite(HKEY hKeyRoot, LPCTSTR szSubPath);

    void Close();

public:
    BOOL GetValue(LPCTSTR szValueName, NString& strValue) const;
    BOOL SetValue(LPCTSTR szValueName, const NString& strValue) const;

    static BOOL GetRegValue(HKEY hKeyRoot, LPCTSTR szSubPath, LPCTSTR szValueName, NString& strValue);
    static BOOL SetRegValue(HKEY hKeyRoot, LPCTSTR szSubPath, LPCTSTR szValueName, const NString& strValue);

private:
    HKEY    m_hKey;
};
