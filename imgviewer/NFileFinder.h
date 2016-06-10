#pragma once

class NFileFinder : protected Noncopyable
{
public:
    NFileFinder(void);
    ~NFileFinder(void);

    // fileExts: .exe|.bat|.png|
    bool Find(LPCTSTR folderPath, LPCTSTR fileName, LPCTSTR fileExts);
    bool Prev(NString& nextFile);
    bool Next(NString& nextFile);

private:
    int fileIndex_;

    NString folderPath_;
    NArrayT<NString> fileNames_;
};
