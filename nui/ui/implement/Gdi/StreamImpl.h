#pragma once


// #define TEST_STREAM

class StreamImpl : public IStream
{
public:
    StreamImpl();
    ~StreamImpl();

public:
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void ** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);


public:
    virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER *plibNewPosition);

    virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER libNewSize);

    virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream *pstm, ULARGE_INTEGER cb, ULARGE_INTEGER *pcbRead, ULARGE_INTEGER *pcbWritten);

    virtual HRESULT STDMETHODCALLTYPE Commit(DWORD grfCommitFlags);

    virtual HRESULT STDMETHODCALLTYPE Revert(void);

    virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);

    virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);

    virtual HRESULT STDMETHODCALLTYPE Stat(__RPC__out STATSTG *pstatstg, DWORD grfStatFlag);

    virtual HRESULT STDMETHODCALLTYPE Clone(__RPC__deref_out_opt IStream **ppstm);

    virtual HRESULT STDMETHODCALLTYPE Read(void *pv, ULONG cb, ULONG *pcbRead);

    virtual HRESULT STDMETHODCALLTYPE Write(const void *pv, ULONG cb, ULONG *pcbWritten);

protected:
#ifdef TEST_STREAM
    IStream*    stream_;
#endif  // TEST_STREAM
    const void*   data_;
    ULONG   length_;
    DWORD   curPos_;
    int refCount_;
};
