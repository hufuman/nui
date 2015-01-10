#include "stdafx.h"
#include "NFileMappingImpl.h"

#include "../../base/NMemTool.h"

IMPLEMENT_REFLECTION(NFileMappingImpl);


NFileMappingImpl::NFileMappingImpl()
{
    file_ = INVALID_HANDLE_VALUE;
    fileMapping_ = NULL;
    data_ = NULL;
    size_ = 0;
}

NFileMappingImpl::~NFileMappingImpl()
{
    Close();
}

bool NFileMappingImpl::OpenForRead(LPCTSTR filePath)
{
    Close();

    for(; ;)
    {
        file_ = ::CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
        if(file_ == INVALID_HANDLE_VALUE)
            break;

        fileMapping_ = ::CreateFileMapping(file_, NULL, PAGE_READONLY, 0, 0, NULL);
        if(fileMapping_ == NULL)
            break;

        data_ = (LPBYTE)::MapViewOfFile(fileMapping_, FILE_MAP_READ, 0, 0, 0);
        if(data_ == NULL)
            break;

        size_ = ::GetFileSize(file_, NULL);
        break;
    }
    if(size_ <= 0)
        Close();

    return (size_ > 0);
}

void NFileMappingImpl::Close()
{
    if(data_ != NULL)
    {
        ::UnmapViewOfFile(data_);
        data_ = NULL;
        size_ = 0;
    }
    if(fileMapping_ != NULL)
    {
        ::CloseHandle(fileMapping_);
        fileMapping_ = NULL;
    }
    if(file_ != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(file_);
        file_ = INVALID_HANDLE_VALUE;
    }
}

LPBYTE NFileMappingImpl::GetData()
{
    return data_;
}

DWORD NFileMappingImpl::GetSize()
{
    return size_;
}
