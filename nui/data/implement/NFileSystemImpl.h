#pragma once


#include "../NFileSystem.h"
#include "NRealFileSystemImpl.h"
#include "NPackFileSystemImpl.h"


class NFileSystemImpl : public nui::Data::NFileSystem
{
public:
    NFileSystemImpl(void);
    virtual ~NFileSystemImpl(void);

    virtual bool Init(LPCTSTR packFilePath);
    virtual void Destroy();

    virtual bool IsFileExists(LPCTSTR filePath);
    virtual bool LoadFile(LPCTSTR filePath, nui::Data::NBuffer* buffer);

protected:
    NRealFileSystemImpl realFS_;
    NPackFileSystemImpl packFS_;
};
