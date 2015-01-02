#include "stdafx.h"
#include "NBufferImpl.h"

#include "../../base/NMemTool.h"

IMPLEMENT_REFLECTION(NBufferImpl);

NBufferImpl::NBufferImpl()
{
    buffer_ = NULL;
    capacity_ = 0;
    size_ = 0;
}

NBufferImpl::~NBufferImpl()
{
    if(buffer_ != NULL)
        ReleaseBuffer();
}

NUI::Data::NBuffer& NBufferImpl::SetSize(DWORD size)
{
    if(capacity_ < size)
    {
        ReleaseBuffer();
        if(size > 0)
        {
            buffer_ = NUI::Base::NMalloc(size);
            if(buffer_ != NULL)
            {
                capacity_ = size;
                size_ = size;
            }
        }
    }
    else
    {
        size_ = size;
    }
    return (*this);
}

DWORD NBufferImpl::GetCapacity() const
{
    return capacity_;
}

DWORD NBufferImpl::GetSize() const
{
    return size_;
}

LPVOID NBufferImpl::GetBuffer()
{
    return buffer_;
}

LPVOID NBufferImpl::GetBuffer(DWORD size)
{
    SetSize(size);
    return buffer_;
}

void NBufferImpl::ReleaseBuffer()
{
    if(buffer_ != NULL)
    {
        NUI::Base::NFree(buffer_);
        size_ = 0;
        capacity_ = 0;
    }
}
