#include "stdafx.h"
#include "../NThread.h"

#include <process.h>
#include <windows.h>


namespace NUI
{
    namespace Base
    {
        NThread::NThread()
        {
            threadHandle_ = 0;
        }

        NThread::~NThread()
        {
            NAssertError(threadHandle_ == 0, TEXT("thread hasn't ended"));
            Stop(0, true);
        }

        bool NThread::Start(ThreadImpl impl)
        {
            NAssertError(threadHandle_ == 0, TEXT("thread is started"));
            threadImpl_ = impl;
            threadHandle_ = (HANDLE)_beginthreadex(0, 0, &NThread::ThreadFunc, static_cast<void*>(this), 0, 0);
            return (threadHandle_ != 0);
        }

        bool NThread::Stop(DWORD timeout, bool force)
        {
            if(threadHandle_ == 0)
                return true;

            bool result = true;
            DWORD dwCode = ::WaitForSingleObject((HANDLE)threadHandle_, timeout);
            if(dwCode == WAIT_TIMEOUT && force)
            {
                ::TerminateThread(threadHandle_, 0);
                ::CloseHandle(threadHandle_);
                threadHandle_ = 0;
            }
            else if(dwCode == WAIT_OBJECT_0)
            {
                ::CloseHandle(threadHandle_);
                threadHandle_ = 0;
            }
            else
            {
                result = false;
            }
            return result;
        }

        bool NThread::Wait(DWORD timeout)
        {
            return (::WaitForSingleObject(threadHandle_, timeout) == WAIT_OBJECT_0);
        }

        unsigned int NThread::ThreadFunc(void* param)
        {
            NThread* pThis = reinterpret_cast<NThread*>(param);
            return (pThis->threadImpl_)(param);
        }

    }
}