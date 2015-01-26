#pragma once


#include "../NFileSystem.h"
#include "NRealFileSystemImpl.h"
#include "NPackFileSystemImpl.h"
#include "NPackageFile.h"


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
    bool useRealFS_;
    NRealFileSystemImpl realFS_;
    NPackFileSystemImpl packFS_;

    struct stFsData
    {
        nui::Base::NString realPath;
    };
    typedef std::map<nui::Base::NString, stFsData> FsDataMap;
    FsDataMap   fsDataMap_;

    NPackageFile    packageFile_;
};
