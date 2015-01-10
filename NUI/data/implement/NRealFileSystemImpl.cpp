#include "stdafx.h"
#include "NRealFileSystemImpl.h"

#include "../../util/NFileUtil.h"


NRealFileSystemImpl::NRealFileSystemImpl()
{
}

NRealFileSystemImpl::~NRealFileSystemImpl()
{
}

bool NRealFileSystemImpl::Init(LPCTSTR packFilePath)
{
    UNREFERENCED_PARAMETER(packFilePath);
    return true;
}

void NRealFileSystemImpl::Destroy()
{
    ;
}

bool NRealFileSystemImpl::IsFileExists(LPCTSTR filePath)
{
    return nui::Util::File::IsFileExists(filePath);
}

bool NRealFileSystemImpl::LoadFile(LPCTSTR filePath, nui::Data::NBuffer* pBuffer)
{
    nui::Base::NInstPtr<nui::Data::NFileMapping> mapping(InstPtrParam);
    if(mapping == NULL)
        return false;
    if(mapping->OpenForRead(filePath))
    {
        if(pBuffer->SetSize(mapping->GetSize()))
        {
            memcmp(pBuffer->GetBuffer(), mapping->GetData(), mapping->GetSize());
            return true;
        }
    }
    return false;
}
