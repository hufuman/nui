#pragma once


#include "../NZip.h"
#include "../NFileMapping.h"
#include "../NBuffer.h"
#include <XZip.h>
#include <XUnzip.h>


class NZipImpl : public nui::Data::NZip
{
public:
    NZipImpl(void);
    virtual ~NZipImpl(void);

    // Zip
    virtual bool ZipFolder(LPCTSTR srcFolder, LPCTSTR outputFile);

    // UnZip
    virtual bool LoadFile(LPCTSTR zipFilePath);
    virtual bool GetFileContent(LPCTSTR relativePath, nui::Data::NBuffer* buffer);
    virtual bool IsFileExists(LPCTSTR relativePath);
    virtual void Close();

protected:
    ZIPENTRYW* GetZipEntry(LPCTSTR relativePath);
    // rootPath & srcFolder should be ended by '\\'
    bool ZipFolderHelper(HZIP zipFile, const nui::Base::NString& srcFolder, const nui::Base::NString& rootPath);

private:
    nui::Base::NInstPtr<nui::Data::NFileMapping> zipBuffer_;
    HZIP          zipFile_;

    typedef std::map<nui::Base::NString, ZIPENTRYW> CacheDataMap;
    CacheDataMap    m_mapCacheData;
};
