#include "stdafx.h"
#include "NFileSystemImpl.h"

#include "../../base/NMemTool.h"

IMPLEMENT_REFLECTION_EX(NFileSystemImpl, nui::Base::NReflect::Singleton);

NFileSystemImpl::NFileSystemImpl()
{
    useRealFS_ = true;
}

NFileSystemImpl::~NFileSystemImpl()
{
}

bool NFileSystemImpl::Init(LPCTSTR packFilePath)
{
    nui::Base::NString packFile(packFilePath);
    useRealFS_ = !nui::Util::File::IsFileExists(packFile.GetData());

    packFile = useRealFS_ ? packFilePath : packFile;

    bool result = useRealFS_ ? realFS_.Init(packFilePath) : packFS_.Init(packFile.GetData());
    if(!result)
        return false;

    packageFile_.Init(packFilePath, useRealFS_);

    LPCTSTR coreFilePath = _T("@root:package.xml");
    nui::Base::NString fullPath = packageFile_.GetRealPath(coreFilePath);
    if(!IsFileExists(fullPath.GetData()))
        return false;

    nui::Base::NInstPtr<nui::Data::NBuffer> fileBuffer(MemToolParam);
    if(!LoadFile(fullPath.GetData(), fileBuffer))
        return false;

    if(!packageFile_.LoadConfig(fileBuffer))
        return false;

    return true;
}

void NFileSystemImpl::Destroy()
{
    if(useRealFS_)
        packFS_.Destroy();
    realFS_.Destroy();
}

bool NFileSystemImpl::IsFileExists(LPCTSTR filePath)
{
    nui::Base::NString realPath = packageFile_.GetRealPath(filePath);
    if(!useRealFS_ && packFS_.IsFileExists(realPath.GetData()))
        return true;
    return realFS_.IsFileExists(realPath.GetData());
}

bool NFileSystemImpl::LoadFile(LPCTSTR filePath, nui::Data::NBuffer* buffer)
{
    nui::Base::NString realPath = packageFile_.GetRealPath(filePath);
    if(!useRealFS_ && packFS_.LoadFile(realPath.GetData(), buffer))
        return true;
    return realFS_.LoadFile(realPath.GetData(), buffer);
}

