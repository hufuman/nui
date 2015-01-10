#pragma once


#include "../NBuffer.h"
#include "../NFileSystem.h"


class NRealFileSystemImpl : public nui::Data::NFileSystem
{
public:
    NRealFileSystemImpl(void);
    virtual ~NRealFileSystemImpl(void);

    virtual bool Init(LPCTSTR packFilePath);
    virtual void Destroy();

    virtual bool IsFileExists(LPCTSTR filePath);
    virtual bool LoadFile(LPCTSTR filePath, nui::Data::NBuffer* pBuffer);
};
