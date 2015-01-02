#include "stdafx.h"
#include "NZipImpl.h"

#include "../../base/NMemTool.h"


using namespace NUI;
using namespace Data;
using namespace Base;

IMPLEMENT_REFLECTION(NZipImpl);


NZipImpl::NZipImpl()
{
    zipBuffer_ = NULL;
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
        if(!NReflectCreate(zipBuffer_))
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
        stCacheData* pCacheData = NNew(stCacheData);
        for(int i=0;  ; ++ i)
        {
            zResult = GetZipItem(zipFile_, i, &pCacheData->entry);
            if(zResult != ZR_OK)
                break;

            strName = pCacheData->entry.name;
            strName.MakeLower();
            pCacheData->buffer = NULL;
            m_mapCacheData.insert(std::make_pair(strName, pCacheData));
            pCacheData = NNew(stCacheData);
        }
        NDelete(pCacheData);
        result = true;
        break;
    }

    if(!result)
    {
        Close();
    }

    return result;
}

bool NZipImpl::GetFileContent(LPCTSTR relativePath, int& index, LPBYTE& data, DWORD& size)
{
    if(relativePath == NULL || relativePath[0] == 0)
        return FALSE;

    if(zipFile_ == NULL)
        return FALSE;

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
        return FALSE;

    stCacheData* pCacheData = ite->second;
    index = pCacheData->entry.index;
    size = pCacheData->entry.unc_size;
    bool result = false;
    if(pCacheData->buffer == NULL || pCacheData->buffer->GetBuffer() == NULL)
    {
        if(pCacheData->buffer != NULL
            || NReflectCreate(pCacheData->buffer))
        {
            LPVOID pBuffer = pCacheData->buffer->GetBuffer(size + 1);
            result = (pBuffer != NULL);
            if(result)
            {
                ZRESULT zResult = UnzipItem(zipFile_, index, pBuffer, size, ZIP_MEMORY);
                result = (zResult == ZR_OK || zResult == ZR_MORE);
                static_cast<LPBYTE>(pBuffer)[size] = 0;
            }
        }
    }
    else
    {
        result = true;
    }
    if(result)
    {
        data = static_cast<LPBYTE>(pCacheData->buffer->GetBuffer());
        pCacheData->buffer->AddRef();
    }
    return result;
}

void NZipImpl::ReleaseFileContent(int index)
{
    CacheDataMap::iterator ite = m_mapCacheData.begin();
    for(; ite != m_mapCacheData.end(); ++ ite)
    {
        if(ite->second->entry.index == index)
            break;
    }

    if(ite == m_mapCacheData.end())
        return;

    if(ite->second->buffer->Release() == 1)
    {
        ite->second->buffer->Release();
        ite->second->buffer = NULL;
    }
}

void NZipImpl::Close()
{
    if(zipBuffer_ != NULL)
    {
        zipBuffer_->Release();
        zipBuffer_ = NULL;
    }
    if(zipFile_ != NULL)
    {
        CloseZip(zipFile_);
        zipFile_ = NULL;
    }
    CacheDataMap::iterator ite = m_mapCacheData.begin();
    for(; ite != m_mapCacheData.end(); ++ ite)
    {
        if(ite->second->buffer != NULL)
        {
            while(ite->second->buffer->Release() > 0)
            {}
        }

        NDelete(ite->second);
    }
    m_mapCacheData.clear();
}

