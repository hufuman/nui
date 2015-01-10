#include "stdafx.h"
#include "MemRecorder.h"



namespace nui
{
    namespace Base
    {
        BOOL NMemLocker::ms_LockAvailable = FALSE;
        CRITICAL_SECTION NMemLocker::ms_Section;
        NMemLocker::NMemLocker()
        {
            if(!ms_LockAvailable)
            {
                ::InitializeCriticalSection(&ms_Section);
                ms_LockAvailable = TRUE;
            }
            if(ms_LockAvailable)
                ::EnterCriticalSection(&ms_Section);
        }
        NMemLocker::~NMemLocker()
        {
            if(ms_LockAvailable)
                ::LeaveCriticalSection(&ms_Section);
        }

        void AddMemLog(nui::Base::NuiMemType memType, void* p, LPCSTR szFilePath, int nLine, LPCSTR szTypeName, int size, int count)
        {
            nui::Implement::NMemRecorder::GetInstance().AddMemLog(memType, p, szFilePath, nLine, szTypeName, size, count);
        }
        void RemoveMemLog(nui::Base::NuiMemType memType, void* p)
        {
            nui::Implement::NMemRecorder::GetInstance().RemoveMemLog(memType, p);
        }
        void CheckMemLeak()
        {
            nui::Implement::NMemRecorder::GetInstance().CheckMemLeak();
        }
        void ReleaseMemChecker()
        {
            nui::Implement::NMemRecorder::GetInstance().ReleaseMemChecker();
        }
    }

    namespace Implement
    {
        NMemRecorder* NMemRecorder::memRecorder_ = new NMemRecorder;

        NMemRecorder::NMemRecorder()
        {
            ;
        }

        NMemRecorder::~NMemRecorder()
        {
            ;
        }

        NMemRecorder& NMemRecorder::GetInstance()
        {
            return *memRecorder_;
        }

        void NMemRecorder::AddMemLog(nui::Base::NuiMemType memType, void* p, LPCSTR szFilePath, int nLine, LPCSTR szTypeName, int size, int count)
        {
            stAllocLog log;
            log.filePath = szFilePath;
            log.typeName = szTypeName;
            log.line = nLine;
            log.size = size;
            log.count = count;
            log.memType = memType;
            mapAllocLog_.insert(std::make_pair(p, log));
        }

        void NMemRecorder::RemoveMemLog(nui::Base::NuiMemType memType, void* p)
        {
            if(this == 0)
                return;
            AllocLogIter iter = FindMemLog(p);
            NAssertError(iter != mapAllocLog_.end(), _T("Ptr Not Found in NMemRecorder::RemoveMemLog"));
            if(iter == mapAllocLog_.end())
                return;
            stAllocLog& log = iter->second;
            NAssertError(log.memType == memType, _T("Wrong Memory Release Function Invoked"));
            mapAllocLog_.erase(iter);
        }

        void NMemRecorder::CheckMemLeak()
        {
            if(mapAllocLog_.size() <= 0)
                return;

            printf("\r\n\r\n//////////////////////////////////////////////////////////////////////////\r\n\r\n");
            printf("Skin Memory Leak Detected: \r\n");
            NAssertError(false, _T("Skin Memory Leak Detected: \r\n"));

            size_t nIndex = 0;
            char logBuffer[4096];
            AllocLogIter iter = mapAllocLog_.begin();
            for(; iter != mapAllocLog_.end(); ++ iter)
            {
                const void* const & pData = iter->first;
                const stAllocLog& log = iter->second;
                const char* memTypeName = "unknown";
                if(log.memType == nui::Base::MemTypeMalloc)
                    memTypeName = "malloc";
                else if(log.memType == nui::Base::MemTypeNew)
                    memTypeName = "new";
                else if(log.memType == nui::Base::MemTypeNewArray)
                    memTypeName = "newArray";
                _snprintf(logBuffer,
                    _countof(logBuffer),
                    "  %4d  Ptr: 0x%p\r\n"
                    "        TypeName: %s, Size: %d, Count: %d\r\n"
                    "        FilePath: %s, Line: %d\r\n"
                    "        Type: %s\r\n",
                    nIndex,
                    pData,
                    log.typeName.c_str(),
                    log.size,
                    log.count,
                    log.filePath.c_str(),
                    log.line,
                    memTypeName);
                printf(logBuffer);
                ++ nIndex;
            }
        }

        void NMemRecorder::ReleaseMemChecker()
        {
            if(memRecorder_ != NULL)
            {
                delete memRecorder_;
                memRecorder_ = NULL;
            }
        }

        NMemRecorder::AllocLogIter NMemRecorder::FindMemLog(void* ptr)
        {
            AllocLogIter iter = mapAllocLog_.find(ptr);
            if(iter == mapAllocLog_.end())
            {
                size_t size = 0;
                iter = mapAllocLog_.begin();
                for(; iter != mapAllocLog_.end(); ++ iter)
                {
                    BYTE * const p = static_cast<BYTE * const>(iter->first);
                    size = iter->second.count * iter->second.size;
                    if(ptr >= p && ptr < p + size)
                    {
                        break;
                    }
                }
            }
            return iter;
        }

    }
}
