#include "stdafx.h"
#include "NPackFileSystemImpl.h"

#include "../../util/NFileUtil.h"


NPackFileSystemImpl::NPackFileSystemImpl() : zipFile_(InstPtrParam)
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
        zipFile_->Close();

    if(!zipFile_->LoadFile(packFilePath))
        return false;
    return true;
}

void NPackFileSystemImpl::Destroy()
{
    zipFile_ = NULL;
}

bool NPackFileSystemImpl::IsFileExists(LPCTSTR filePath)
{
    NAssertError(zipFile_ != NULL, _T("Invalid Zip File"));
    if(!zipFile_)
        return false;
    return zipFile_->IsFileExists(filePath);
}

bool NPackFileSystemImpl::LoadFile(LPCTSTR filePath, nui::Data::NBuffer* buffer)
{
    NAssertError(zipFile_ != NULL, _T("Invalid Zip File"));
    if(!zipFile_)
        return false;
    if(!buffer)
        return false;
    return zipFile_->GetFileContent(filePath, buffer);
}
