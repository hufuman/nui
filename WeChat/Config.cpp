#include "StdAfx.h"
#include "Config.h"

Config::Config(void)
{
}

Config::~Config(void)
{
}

Config& Config::Get()
{
    static Config config;
    return config;
}

NString Config::GetAppDataPath(LPCTSTR pathName)
{
    if(!dataPath_.IsEmpty())
        return dataPath_;

    TCHAR dataPath[1024];
    ::SHGetSpecialFolderPath(NULL, dataPath, CSIDL_APPDATA, FALSE);

    dataPath_ = File::CombinePath(dataPath, _T("WeChatBot"));
    dataPath_ = File::CombinePath(dataPath_, pathName);
    if(!File::MakeSureFolderExists(dataPath_))
    {
        ::GetTempPath(_countof(dataPath), dataPath);
        dataPath_ = dataPath;
    }

    return dataPath_;
}
