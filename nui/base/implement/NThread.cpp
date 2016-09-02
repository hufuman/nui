#include "stdafx.h"
#include "../NThread.h"

#include <process.h>
#include <windows.h>


namespace nui
{
    namespace Base
    {
        NThread::NThread()
        {
            threadHandle_ = 0;
            stopping_ = false;
        }

        NThread::~NThread()
        {
            NAssertError(threadHandle_ == 0, TEXT("thread hasn't ended"));
            StopAndWait();
        }

        bool NThread::Start(ThreadImpl impl)
        {
            NAssertError(threadHandle_ == 0, TEXT("thread is started"));
            threadImpl_ = impl;
            stopping_ = false;
            threadHandle_ = (HANDLE)_beginthreadex(0, 0, &NThread::ThreadFunc, static_cast<void*>(this), 0, 0);
            return (threadHandle_ != 0);
        }

        bool NThread::StopAndWait()
        {
            if(threadHandle_ == 0)
                return true;

            stopping_ = true;
            bool result = true;
            DWORD dwCode = ::WaitForSingleObject((HANDLE)threadHandle_, INFINITE);
            if(dwCode == WAIT_OBJECT_0)
            {
                ::CloseHandle(threadHandle_);
                threadHandle_ = 0;
            }
            else
            {
                NAssertError(false, _T("failed to wait thread"));
                result = false;
            }
            return result;
        }

        unsigned int NThread::ThreadFunc(void* param)
        {
            NThread* pThis = reinterpret_cast<NThread*>(param);
            (pThis->threadImpl_)(pThis->stopping_);
            return 0;
        }

    }
}