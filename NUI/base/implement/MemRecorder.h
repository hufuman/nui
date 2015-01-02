#pragma once

#include "../NMemTool.h"

namespace NUI
{
    namespace Implement
    {
        class NMemRecorder : NUI::Base::Noncopyable
        {
            struct stAllocLog
            {
                std::string filePath;
                int         line;
                std::string typeName;
                int         size;
                int         count;
                NUI::Base::NuiMemType  memType;
            };
            typedef std::map<void*, stAllocLog>     AllocLogMap;
            typedef AllocLogMap::iterator AllocLogIter;

            NMemRecorder();
        public:
            ~NMemRecorder();

            static NMemRecorder& GetInstance();

            void AddMemLog(NUI::Base::NuiMemType memType, void* p, LPCSTR szFilePath, int nLine, LPCSTR szTypeName, int size, int count);
            void RemoveMemLog(NUI::Base::NuiMemType memType, void* p);
            void CheckMemLeak();
            void ReleaseMemChecker();

        private:
            AllocLogIter FindMemLog(void* p);

        private:
            AllocLogMap mapAllocLog_;
            static NMemRecorder* memRecorder_;
        };
    }
}
