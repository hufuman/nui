#include "stdafx.h"
#include "NFileSystemImpl.h"

#include "../../base/NMemTool.h"

IMPLEMENT_REFLECTION_EX(NFileSystemImpl, nui::Base::NReflect::Singleton);

NFileSystemImpl::NFileSystemImpl()
{
}

NFileSystemImpl::~NFileSystemImpl()
{
}

bool NFileSystemImpl::Init(LPCTSTR packFilePath)
{
    return realFS_.Init(packFilePath)
        && packFS_.Init(packFilePath);
}

void NFileSystemImpl::Destroy()
{
    packFS_.Destroy();
    realFS_.Destroy();
}

bool NFileSystemImpl::IsFileExists(LPCTSTR filePath)
{
    return packFS_.IsFileExists(filePath)
        || realFS_.IsFileExists(filePath);
}

bool NFileSystemImpl::LoadFile(LPCTSTR filePath, nui::Data::NBuffer* buffer)
{
    return packFS_.LoadFile(filePath, buffer)
        || realFS_.LoadFile(filePath, buffer);
}

