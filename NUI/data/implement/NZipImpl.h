#pragma once


#include "../NZip.h"
#include "../NFileMapping.h"
#include "../NBuffer.h"
#include <XZip.h>
#include <XUnzip.h>


class NZipImpl : public NUI::Data::NZip
{
public:
    NZipImpl(void);
    virtual ~NZipImpl(void);

    // Zip
    virtual bool ZipFolder(LPCTSTR srcFolder, LPCTSTR outputFile);

    // UnZip
    virtual bool LoadFile(LPCTSTR zipFilePath);
    virtual bool GetFileContent(LPCTSTR relativePath, int& index, LPBYTE& data, DWORD& size);
    virtual void ReleaseFileContent(int index);
    virtual void Close();

private:
    NUI::Data::NFileMapping* zipBuffer_;
    HZIP          zipFile_;

    struct stCacheData
    {
        ZIPENTRYW   entry;
        NUI::Data::NBuffer*  buffer;
    };
    typedef std::map<NUI::Base::NString, stCacheData*> CacheDataMap;
    CacheDataMap    m_mapCacheData;
};
