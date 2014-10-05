#pragma once


#include "../NBuffer.h"


class NBufferImpl : public NUI::Data::NBuffer
{
public:
    NBufferImpl(void);
    virtual ~NBufferImpl(void);

    virtual NBuffer& SetSize(DWORD size);
    virtual DWORD GetSize() const;
    virtual LPVOID GetBuffer();
    virtual LPVOID GetBuffer(DWORD size);
    virtual void ReleaseBuffer();

protected:
    LPVOID buffer_;
    DWORD  capacity_;
    DWORD  size_;
};
