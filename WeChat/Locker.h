#pragma once


class ILocker
{
public:
    virtual ~ILocker(){}

    virtual void Lock() = 0;
    virtual void UnLock() = 0;
};

class CSectionLocker : public ILocker
{
    CSectionLocker(const CSectionLocker&);
    CSectionLocker& operator = (const CSectionLocker&);
public:
    CSectionLocker()
    {
        ::InitializeCriticalSection(&section_);
    }

    ~CSectionLocker()
    {
        ::DeleteCriticalSection(&section_);
    }

    virtual void Lock()
    {
        ::EnterCriticalSection(&section_);
    }

    virtual void UnLock()
    {
        ::LeaveCriticalSection(&section_);
    }

private:
    CRITICAL_SECTION section_;
};

class CAutoLocker
{
    CAutoLocker();
    CAutoLocker(const CAutoLocker&);
    CAutoLocker& operator = (const CAutoLocker&);
public:
    CAutoLocker(ILocker* locker)
    {
        locker_ = locker;
        locker_->Lock();
    }

    ~CAutoLocker()
    {
        locker_->UnLock();
    }

private:
    ILocker* locker_;
};
