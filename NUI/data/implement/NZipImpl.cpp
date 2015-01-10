#include "stdafx.h"
#include "NZipImpl.h"

#include "../../base/NMemTool.h"


using namespace nui;
using namespace Data;
using namespace Base;

IMPLEMENT_REFLECTION(NZipImpl);


NZipImpl::NZipImpl() : zipBuffer_(InstPtrParam)
{
    zipFile_ = NULL;
}

NZipImpl::~NZipImpl(void)
{
    Close();
}

// Zip
bool NZipImpl::ZipFolder(LPCTSTR srcFolder, LPCTSTR outputFile)
{
    if(srcFolder == NULL || srcFolder[0] == 0)
        return false;

    DWORD fileAttr = ::GetFileAttributes(srcFolder);
    if(fileAttr == INVALID_FILE_ATTRIBUTES)
        return false;

    if((fileAttr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
        return false;

    HZIP zipFile = CreateZip(reinterpret_cast<void*>(const_cast<LPTSTR>(outputFile)), 0, ZIP_FILENAME);
    if(zipFile == NULL)
        return false;

    NString strFolder(srcFolder);
    if(strFolder[strFolder.GetLength() - 1] == _T('\\'))
        strFolder.Resize(strFolder.GetLength() - 1);

    NString strName;
    size_t pos = strFolder.LastIndexOf(_T('\\'));
    if(pos == -1)
        return false;

    strName = strFolder.SubString(pos + 1);
    strFolder.Resize(pos);

    bool result = !!AddFolderContent(zipFile, strFolder.GetData(), strName.GetData());
    result =(ZR_OK == CloseZip(zipFile)) && result;

    if(!result)
    {
        DWORD fileAttr = ::GetFileAttributes(outputFile);
        if(fileAttr != INVALID_FILE_ATTRIBUTES)
            ::DeleteFile(outputFile);
    }

    return result;
}

// UnZip
bool NZipImpl::LoadFile(LPCTSTR zipFilePath)
{
    Close();

    bool result = false;
    for (;;)
    {
        if(zipBuffer_.Create(InstPtrParam) == NULL)
            break;

        if(!zipBuffer_->OpenForRead(zipFilePath))
            break;

        DWORD dwSize = zipBuffer_->GetSize();
        LPBYTE pData = zipBuffer_->GetData();
        if(pData == NULL)
            break;

        zipFile_ = OpenZip(pData, dwSize, ZIP_MEMORY);
        if(zipFile_ == NULL)
            break;

        ZRESULT zResult = ZR_OK;
        NString strName;
        for(int i=0;  ; ++ i)
        {
            ZIPENTRYW entry;
            zResult = GetZipItem(zipFile_, i, &entry);
            if(zResult != ZR_OK)
                break;

            strName = entry.name;
            strName.MakeLower();
            m_mapCacheData.insert(std::make_pair(strName, entry));
        }
        result = true;
        break;
    }

    if(!result)
    {
        Close();
    }

    return result;
}

bool NZipImpl::GetFileContent(LPCTSTR relativePath, nui::Data::NBuffer* buffer)
{
    if(buffer == NULL)
        return false;

    ZIPENTRYW* entry = GetZipEntry(relativePath);
    if(!entry)
        return false;

    bool result = false;
    LPVOID pBuffer = buffer->GetBuffer(entry->unc_size + 1);
    result = (pBuffer != NULL);
    if(result)
    {
        ZRESULT zResult = UnzipItem(zipFile_, entry->index, pBuffer, entry->unc_size, ZIP_MEMORY);
        result = (zResult == ZR_OK || zResult == ZR_MORE);
        static_cast<LPBYTE>(pBuffer)[entry->unc_size] = 0;
    }
    return result;
}

bool NZipImpl::IsFileExists(LPCTSTR relativePath)
{
    ZIPENTRYW* entry = GetZipEntry(relativePath);
    return entry != NULL;
}

void NZipImpl::Close()
{
    zipBuffer_ = NULL;
    if(zipFile_ != NULL)
    {
        CloseZip(zipFile_);
        zipFile_ = NULL;
    }
    m_mapCacheData.clear();
}


ZIPENTRYW* NZipImpl::GetZipEntry(LPCTSTR relativePath)
{
    if(relativePath == NULL || relativePath[0] == 0)
        return NULL;

    if(zipFile_ == NULL)
        return NULL;

    NString strRelativePath(relativePath);
    strRelativePath.MakeLower();
    strRelativePath.Replace(_T("\\"), _T("/"));

    CacheDataMap::iterator ite = m_mapCacheData.begin();
    for(; ite != m_mapCacheData.end(); ++ ite)
    {
        if(ite->first == strRelativePath)
            break;
    }

    if(ite == m_mapCacheData.end())
        return NULL;
    return &ite->second;
}