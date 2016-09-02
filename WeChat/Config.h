#pragma once

class Config
{
    Config();
    Config(const Config&);
    Config& operator = (const Config&);
public:
    ~Config(void);

    static Config& Get();

public:
    NString GetAppDataPath(LPCTSTR pathName);

private:
    NString dataPath_;
};
