#pragma once


#include "../NFileMapping.h"


class NFileMappingImpl : public nui::Data::NFileMapping
{
public:
    NFileMappingImpl(void);
    virtual ~NFileMappingImpl(void);

    virtual bool OpenForRead(LPCTSTR filePath);
    virtual void Close();
    virtual LPBYTE GetData();
    virtual DWORD GetSize();

protected:
    HANDLE file_;
    HANDLE fileMapping_;
    LPBYTE data_;
    DWORD  size_;
};
