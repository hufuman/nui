#include "StdAfx.h"
#include "Utf8File.h"

#pragma warning(disable: 4996)

class CFileHandler
{
    CFileHandler();
    CFileHandler(const CFileHandler&);
    CFileHandler& operator = (const CFileHandler&);
public:
    CFileHandler(HANDLE handle) : file_(handle)
    {
    }

    ~CFileHandler()
    {
        if(file_ != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(file_);
            file_ = INVALID_HANDLE_VALUE;
        }
    }

    operator bool () const
    {
        return (file_ != INVALID_HANDLE_VALUE);
    }

    operator HANDLE() const
    {
        return file_;
    }

private:
    HANDLE file_;
};

CUtf8File::CUtf8File(void)
{
    buffer_ = NULL;
    length_ = 0;
}

CUtf8File::~CUtf8File(void)
{
    Clear();
}

bool CUtf8File::LoadFile(LPCTSTR filePath)
{
    CFileHandler handle(::CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL));
    if(!handle)
        return false;
    DWORD fileSize = ::GetFileSize(handle, NULL);
    char* buffer = NNewArray(char, fileSize);
    if(buffer == NULL)
        return false;

    DWORD readSize;
    if(!::ReadFile(handle, buffer, fileSize, &readSize, NULL) || readSize != fileSize)
    {
        NDeleteArray(buffer);
        return false;
    }

#pragma warning(disable: 4309)
    const char utf8Bom[] = {0xEF, 0xBB, 0xBF};
    const char utf16Bom[] = {0xFF, 0xFE};

    if(fileSize >= 3 && buffer[0] == utf8Bom[0] && buffer[1] == utf8Bom[1] && buffer[2] == utf8Bom[2])
    {
        // Utf 8
        length_ = fileSize - 3;
        buffer_ = NNewArray(char, length_ + 1);
        strncpy(buffer_, buffer + 3, length_);
        buffer_[length_] = 0;
    }
    else if(fileSize >= 2 && buffer[0] == utf16Bom[0] && buffer[1] == utf16Bom[1])
    {
        // Utf 16
        std::string content = Data::t2utf8(reinterpret_cast<LPTSTR>(buffer + 2), fileSize / 2 - 1);
        buffer_ = NNewArray(char, content.length() + 1);
        length_ = content.length();
        strncpy(buffer_, content.c_str(), length_);
        buffer_[length_] = 0;
    }
    else
    {
        // No bom
        std::string content = Data::a2utf8(buffer, fileSize);
        buffer_ = NNewArray(char, content.length() + 1);
        length_ = content.length();
        strncpy(buffer_, content.c_str(), length_);
        buffer_[length_] = 0;
    }

    NDeleteArray(buffer);
    return true;
}

const char* CUtf8File::GetBuffer() const
{
    return buffer_;
}

int CUtf8File::GetLength() const
{
    return length_;
}

void CUtf8File::Clear()
{
    if(buffer_ == NULL)
        return;

    NDeleteArray(buffer_);
    buffer_ = 0;
    length_ = 0;
}
