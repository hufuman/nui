#include "stdafx.h"
#include "StreamImpl.h"



StreamImpl::StreamImpl()
{
#ifdef TEST_STREAM
    stream_ = NULL;
    CreateStreamOnHGlobal(NULL, TRUE, &stream_);
#endif // TEST_STREAM

    data_ = NULL;
    length_ = 0;
    curPos_ = 0;
    refCount_ = 0;
}

StreamImpl::~StreamImpl()
{
#ifdef TEST_STREAM
    stream_->Release();
    stream_ = NULL;
#endif // TEST_STREAM

    data_ = NULL;
    length_ = 0;
    curPos_ = 0;
    refCount_ = 0;
}

HRESULT StreamImpl::QueryInterface(REFIID riid, void ** ppvObject)
{
    *ppvObject = NULL;

    HRESULT hResult = E_NOINTERFACE;
    if(IsEqualIID(riid, __uuidof(IUnknown)))
    {
        *ppvObject = dynamic_cast<IUnknown*>(this);
        hResult = S_OK;
    }
    else if(IsEqualIID(riid, __uuidof(ISequentialStream)))
    {
        *ppvObject = dynamic_cast<ISequentialStream*>(this);
        hResult = S_OK;
    }
    else if(IsEqualIID(riid, __uuidof(IStream)))
    {
        *ppvObject = dynamic_cast<IStream*>(this);
        hResult = S_OK;
    }

    if(SUCCEEDED(hResult) && *ppvObject != NULL)
    {
        AddRef();
    }

    return hResult;
}

ULONG StreamImpl::AddRef(void)
{
    return (++ refCount_);
}

ULONG StreamImpl::Release(void)
{
    if((-- refCount_) <= 0)
    {
        NDeleteThis(this);
        return 0;
    }
    return refCount_;
}

HRESULT StreamImpl::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition)
{
#ifdef TEST_STREAM
    ULARGE_INTEGER uResult;
    DWORD dwCurPos = curPos_;
    HRESULT hResult = stream_->Seek(dlibMove, dwOrigin, &uResult);
#endif // TEST_STREAM

    switch(dwOrigin)
    {
    case STREAM_SEEK_SET:
        curPos_ = (int)dlibMove.LowPart;
        break;

    case STREAM_SEEK_CUR:
        curPos_ += dlibMove.LowPart;
        break;

    case STREAM_SEEK_END:
        curPos_ = length_ - (int)dlibMove.LowPart;
        break;
    }

    if(curPos_ < 0 || curPos_ > length_)
        return STG_E_INVALIDFUNCTION;

    if(plibNewPosition != NULL)
    {
        plibNewPosition->HighPart = 0;
        plibNewPosition->LowPart = curPos_;
    }

#ifdef TEST_STREAM
    if(plibNewPosition != NULL && (memcmp(&uResult, plibNewPosition, sizeof(uResult)) != 0))
    {
        int a = 0;
    }
#endif // TEST_STREAM
    return S_OK;
}

HRESULT StreamImpl::Stat(__RPC__out STATSTG *pstatstg, DWORD grfStatFlag)
{
    UNREFERENCED_PARAMETER(grfStatFlag);
#ifdef TEST_STREAM
    STATSTG statstg;
    HRESULT hResult = stream_->Stat(&statstg, grfStatFlag);
#endif // TEST_STREAM

    memset(pstatstg, 0, sizeof(STATSTG));
    pstatstg->cbSize.HighPart = 0;
    pstatstg->cbSize.LowPart = length_;
    pstatstg->type = STGTY_STREAM;

#ifdef TEST_STREAM
    if(memcmp(&statstg, pstatstg, sizeof(statstg)) != 0)
    {
        int a = 0;
    }
#endif // TEST_STREAM

    return S_OK;
}

HRESULT StreamImpl::Read(void *pv, ULONG cb, ULONG *pcbRead)
{
#ifdef TEST_STREAM
    void* p = malloc(cb);
    ULONG uRightRead = 0;
    HRESULT hResult = stream_->Read(p, cb, &uRightRead);
#endif // TEST_STREAM

    DWORD dwRead = 0;
    dwRead = (curPos_ + cb >= length_) ? length_ - curPos_ : cb;
    memcpy(pv, (BYTE*)data_ + curPos_, dwRead);
    curPos_ += dwRead;
    if(pcbRead != NULL)
    {
        *pcbRead = dwRead;
    }

#ifdef TEST_STREAM
    if(pcbRead != NULL)
    {
        if(*pcbRead != uRightRead)
        {
            int a = 0;
        }
    }
    if(memcmp(pv, p, uRightRead) != 0)
    {
        int a = 0;
    }
    free(p);
#endif // TEST_STREAM

    return S_OK;
}

HRESULT StreamImpl::Write(const void *pv, ULONG cb, ULONG *pcbWritten)
{
#ifdef TEST_STREAM
    HRESULT hResult = stream_->Write(pv, cb, pcbWritten);
#endif // TEST_STREAM

    data_ = pv;
    length_ = cb;
    curPos_ = cb;
    if(pcbWritten != NULL)
    {
        *pcbWritten = cb;
    }
    return S_OK;
}


HRESULT StreamImpl::SetSize(ULARGE_INTEGER libNewSize)
{
    UNREFERENCED_PARAMETER(libNewSize);
    return E_NOTIMPL;
}

HRESULT StreamImpl::CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten)
{
    UNREFERENCED_PARAMETER(pstm);
    UNREFERENCED_PARAMETER(cb);
    UNREFERENCED_PARAMETER(pcbRead);
    UNREFERENCED_PARAMETER(pcbWritten);
    return E_NOTIMPL;
}

HRESULT StreamImpl::Commit(DWORD grfCommitFlags)
{
    UNREFERENCED_PARAMETER(grfCommitFlags);
    return E_NOTIMPL;
}

HRESULT StreamImpl::Revert(void)
{
    return E_NOTIMPL;
}

HRESULT StreamImpl::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    UNREFERENCED_PARAMETER(libOffset);
    UNREFERENCED_PARAMETER(cb);
    UNREFERENCED_PARAMETER(dwLockType);
    return E_NOTIMPL;
}

HRESULT StreamImpl::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    UNREFERENCED_PARAMETER(libOffset);
    UNREFERENCED_PARAMETER(cb);
    UNREFERENCED_PARAMETER(dwLockType);
    return E_NOTIMPL;
}

HRESULT StreamImpl::Clone(__RPC__deref_out_opt IStream **ppstm)
{
    UNREFERENCED_PARAMETER(ppstm);
    return E_NOTIMPL;
}