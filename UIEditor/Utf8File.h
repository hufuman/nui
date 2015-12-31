#pragma once

class CUtf8File
{
public:
    CUtf8File(void);
    ~CUtf8File(void);

    bool LoadFile(LPCTSTR filePath);

    const char* GetBuffer() const;
    int GetLength() const;

private:
    void Clear();

private:
    char* buffer_;
    int length_;
};
