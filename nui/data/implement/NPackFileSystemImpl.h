#pragma once


#include "../NFileSystem.h"
#include "../NZip.h"


class NPackFileSystemImpl : public nui::Data::NFileSystem
{
public:
    NPackFileSystemImpl(void);
    virtual ~NPackFileSystemImpl(void);

    virtual bool Init(LPCTSTR packFilePath);
    virtual void Destroy();

    virtual bool IsFileExists(LPCTSTR filePath);
    virtual bool LoadFile(LPCTSTR filePath, nui::Data::NBuffer* buffer);

protected:
    nui::Base::NString GetPackPath(LPCTSTR filePath);

protected:
    nui::Data::NZip* zipFile_;
};
