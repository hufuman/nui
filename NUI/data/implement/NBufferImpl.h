#pragma once


#include "../NBuffer.h"


class NBufferImpl : public nui::Data::NBuffer
{
public:
    NBufferImpl(void);
    virtual ~NBufferImpl(void);

    virtual bool SetSize(DWORD size);
    virtual DWORD GetCapacity() const;
    virtual DWORD GetSize() const;
    virtual LPVOID GetBuffer();
    virtual LPVOID GetBuffer(DWORD size);
    virtual void ReleaseBuffer();

protected:
    LPVOID buffer_;
    DWORD  capacity_;
    DWORD  size_;
};
