#pragma once

class FileAssoc
{
public:
    FileAssoc(void);
    ~FileAssoc(void);

    bool Start();

    // .exe
    bool Assoc(LPCTSTR fileExt);

    bool Roll(BOOL needAssoc);

private:
    NArrayT<NString> fileExts_;
};
