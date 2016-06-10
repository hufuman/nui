#include "StdAfx.h"
#include "NFileFinder.h"

NFileFinder::NFileFinder(void)
{
}

NFileFinder::~NFileFinder(void)
{
}

bool NFileFinder::Find(LPCTSTR folderPath, LPCTSTR fileName, LPCTSTR fileExts)
{
    fileNames_.Clear();

    WIN32_FIND_DATA findData;

    NString lowerFileExts(fileExts);
    NString filter = Util::File::CombinePath(folderPath, _T("*.*"));
    HANDLE findHandle = ::FindFirstFile(filter.GetData(), &findData);
    if(findHandle == INVALID_HANDLE_VALUE)
        return false;

    int pos;
    NString tmp;
    folderPath_ = folderPath;
    do 
    {
        tmp = findData.cFileName;
        pos = tmp.LastIndexOf(_T("."));
        if(pos <= 0)
            continue;

        tmp = tmp.SubString(pos);
        tmp += _T(";");

        if(lowerFileExts.IndexOf(tmp.ToLower()) <= 0)
            continue;

        if(_tcsicmp(fileName, findData.cFileName) == 0)
            fileIndex_ = fileNames_.Count();

        fileNames_.AddItem(findData.cFileName);
    } while (::FindNextFile(findHandle, &findData));
    ::FindClose(findHandle);
    return fileNames_.Count() > 0;
}

bool NFileFinder::Prev(NString& nextFile)
{
    if(fileNames_.Count() == 0)
        return false;

    -- fileIndex_;
    if(fileIndex_ < 0)
        fileIndex_ = fileNames_.Count() - 1;
    nextFile = File::CombinePath(folderPath_, fileNames_[fileIndex_]);
    return true;
}

bool NFileFinder::Next(NString& nextFile)
{
    if(fileNames_.Count() == 0)
        return false;

    ++ fileIndex_;
    if(fileIndex_ >= fileNames_.Count())
        fileIndex_ = 0;
    nextFile = File::CombinePath(folderPath_, fileNames_[fileIndex_]);
    return true;
}