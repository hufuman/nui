#include "stdafx.h"
#include "NZipImpl.h"

#include "../../base/NMemTool.h"
#include "../../util/NFileUtil.h"


using namespace nui;
using namespace Data;
using namespace Base;
using namespace Util;

IMPLEMENT_REFLECTION(NZipImpl);


NZipImpl::NZipImpl() : zipBuffer_(MemToolParam)
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
    if(srcFolder == NULL || srcFolder[0] == 0 || outputFile == NULL || outputFile[0] == 0)
        return false;

    DWORD fileAttr = ::GetFileAttributes(srcFolder);
    if(fileAttr == INVALID_FILE_ATTRIBUTES)
        return false;

    if((fileAttr & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
        return false;

    HZIP zipFile = CreateZip(reinterpret_cast<void*>(const_cast<LPTSTR>(outputFile)), 0, ZIP_FILENAME);
    if(zipFile == NULL)
        return false;

    NString folder = File::CombinePath(srcFolder, _T(""));

    bool result = ZipFolderHelper(zipFile, folder, folder);
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
        if(zipBuffer_.Create(MemToolParam) == NULL)
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
        buffer->SetSize(buffer->GetSize() - 1);
        ZRESULT zResult = UnzipItem(zipFile_, entry->index, pBuffer, entry->unc_size, ZIP_MEMORY);
        result = (zResult == ZR_OK);
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

bool NZipImpl::ZipFolderHelper(HZIP zipFile, const NString& srcFolder, const NString& rootPath)
{
    NString filter = srcFolder + _T("*");

    WIN32_FIND_DATA data;
    HANDLE findHandle = ::FindFirstFile(filter, &data);
    if(findHandle == INVALID_HANDLE_VALUE)
        return false;

    bool result = true;
    NString absPath;
    do
    {
        if(_tcscmp(data.cFileName, _T(".")) == 0
            || _tcscmp(data.cFileName, _T("..")) == 0)
        {
            continue;
        }
        absPath = File::CombinePath(srcFolder, data.cFileName);
        DWORD fileAttr = ::GetFileAttributes(absPath);
        bool isDir = ((fileAttr & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
        if(isDir)
        {
            result = (ZR_OK == ZipAdd(zipFile, absPath.SubString(rootPath.GetLength()), 0, 0, ZIP_FOLDER));
            if(result)
                result = ZipFolderHelper(zipFile, absPath + _T("\\"), rootPath);
        }
        else
        {
            result = (ZR_OK == ZipAdd(zipFile, absPath.SubString(rootPath.GetLength()), (void*)(LPCTSTR)absPath, 0, ZIP_FILENAME));
        }
    } while(result && ::FindNextFile(findHandle, &data));
    ::FindClose(findHandle);

    return result;
}