#include "StdAfx.h"
#include "FileAssoc.h"

#include "Registry.h"

FileAssoc::FileAssoc(void)
{
}

FileAssoc::~FileAssoc(void)
{
}

bool FileAssoc::Start()
{
    fileExts_.RemoveAllItems();
    return false;
}

// .exe
bool FileAssoc::Assoc(LPCTSTR fileExt)
{
    if(fileExt == NULL || fileExt[0] != _T('.'))
        return false;
    int count = fileExts_.Count();
    for(int i=0; i<count; ++ i)
    {
        if(fileExts_[i] == fileExt)
            return true;
    }
    fileExts_.AddItem(fileExt);
    return true;
}

bool FileAssoc::Roll(BOOL needAssoc)
{
    // Ensure Self Info
    NString selfPath, tmpValue, tmpPath;

    if(needAssoc)
    {
        selfPath = NModule::GetInst().GetAppFullName() + _T(" \"%1\"");
        LPCTSTR szSelfCmdPath = _T("ImgViewer.Open\\shell\\open\\command");
        if(!CRegistry::GetRegValue(HKEY_CLASSES_ROOT, szSelfCmdPath, _T(""), tmpPath)
            || tmpPath.IndexOf(selfPath) != 0)
        {
            if(!CRegistry::SetRegValue(HKEY_CLASSES_ROOT, szSelfCmdPath, _T(""), selfPath))
                return false;
        }
    }

    bool result = true;
    LPCTSTR szRegName = _T("ImgViewer.Open");
    LPCTSTR szBackupName = _T("ImgViewer.Backup");

    if(needAssoc)
    {
        int count = fileExts_.Count();
        for(int i=0; i<count; ++ i)
        {
            if(!CRegistry::GetRegValue(HKEY_CLASSES_ROOT, fileExts_[i], _T(""), tmpValue)
                || tmpValue != szRegName)
            {
                // backup org value
                if(CRegistry::SetRegValue(HKEY_CLASSES_ROOT, fileExts_[i], szBackupName, tmpValue))
                    result = CRegistry::SetRegValue(HKEY_CLASSES_ROOT, fileExts_[i], _T(""), szRegName) && result;
            }
        }
    }
    else
    {
        NString orgAssoc;
        int count = fileExts_.Count();
        for(int i=0; i<count; ++ i)
        {
            if(!CRegistry::GetRegValue(HKEY_CLASSES_ROOT, fileExts_[i], szBackupName, orgAssoc))
                continue;

            if(!CRegistry::GetRegValue(HKEY_CLASSES_ROOT, fileExts_[i], _T(""), tmpValue)
                || tmpValue != orgAssoc)
            {
                result = CRegistry::SetRegValue(HKEY_CLASSES_ROOT, fileExts_[i], _T(""), orgAssoc) && result;
            }
        }
    }

    return result;
}
