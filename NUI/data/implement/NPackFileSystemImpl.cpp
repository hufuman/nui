#include "stdafx.h"
#include "NPackFileSystemImpl.h"

#include "../../util/NFileUtil.h"


NPackFileSystemImpl::NPackFileSystemImpl()
{
}

NPackFileSystemImpl::~NPackFileSystemImpl()
{
}

bool NPackFileSystemImpl::Init(LPCTSTR packFilePath)
{
    if(!nui::Util::File::IsFileExists(packFilePath))
        return false;
    if(zipFile_)
    {
        Destroy();
    }
    if(!zipFile_->LoadFile(packFilePath))
        return false;
    return true;
}

void NPackFileSystemImpl::Destroy()
{
    NAssertError(zipFile_->RefCount() == 1, _T("Some Files Referenced"));
    NSafeRelease(zipFile_);
}

bool NPackFileSystemImpl::IsFileExists(LPCTSTR filePath)
{
    NAssertError(zipFile_ != NULL, _T("Invalid Zip File"));
    if(!zipFile_)
        return false;
    nui::Base::NString packPath = GetPackPath(filePath);
    return zipFile_->IsFileExists(packPath.GetData());
}

bool NPackFileSystemImpl::LoadFile(LPCTSTR filePath, nui::Data::NBuffer* buffer)
{
    NAssertError(zipFile_ != NULL, _T("Invalid Zip File"));
    if(!zipFile_)
        return false;
    if(!buffer)
        return false;
    nui::Base::NString packPath = GetPackPath(filePath);
    return zipFile_->GetFileContent(packPath.GetData(), buffer);
}

nui::Base::NString NPackFileSystemImpl::GetPackPath(LPCTSTR filePath)
{
    UNREFERENCED_PARAMETER(filePath);
    throw "Not Implement";
}
