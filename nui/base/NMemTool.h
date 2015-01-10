#pragma once


namespace nui
{
    namespace Base
    {
        enum NuiMemType
        {
            MemTypeMalloc,
            MemTypeNew,
            MemTypeNewArray,
        };

        NUI_API void AddMemLog(NuiMemType memType, void* p, LPCSTR szFilePath, int nLine, LPCSTR szTypeName, int size, int count);
        NUI_API void RemoveMemLog(NuiMemType memType, void* p);
        NUI_API void CheckMemLeak();
        NUI_API void ReleaseMemChecker();

        class NUI_CLASS NMemLocker
        {
        public:
            NMemLocker();
            ~NMemLocker();

        private:
            static BOOL ms_LockAvailable;
            static CRITICAL_SECTION ms_Section;
        };

#define NMalloc(size) NMallocImpl(size, __FILE__, __LINE__)
        __inline void* NMallocImpl(int size, LPCSTR szFilePath, int nLine)
        {
            NMemLocker locker;
            void* p = malloc(size);
            AddMemLog(MemTypeMalloc, p, szFilePath, nLine, "malloc", size, 1);
            return p;
        }

        __inline void NFree(void*& ptr)
        {
            if(ptr == NULL)
                return;
            NMemLocker locker;
            RemoveMemLog(MemTypeMalloc, ptr);
            free(ptr);
            ptr = NULL;
        }

#define NNew(type, ...) NNewImpl<type>(nui::Base::MemTypeNew, new type(__VA_ARGS__), 1, __FILE__, __LINE__)
#define NNewArray(type, count) NNewImpl<type>(nui::Base::MemTypeNewArray, new type[count], count, __FILE__, __LINE__)
        template <typename T>
        T* NNewImpl(NuiMemType memType, T* p, int count, LPCSTR szFilePath, int nLine)
        {
            NMemLocker locker;
            AddMemLog(memType, p, szFilePath, nLine, typeid(T).name(), sizeof(T), count);
            return p;
        }

        template <typename T>
        void NDeleteThis(T* ptr)
        {
            if(ptr == NULL)
                return;
            NMemLocker locker;
            RemoveMemLog(MemTypeNew, ptr);
            delete ptr;
        }

        template <typename T>
        void NDelete(T*& ptr)
        {
            if(ptr == NULL)
                return;
            NMemLocker locker;
            RemoveMemLog(MemTypeNew, ptr);
            delete ptr;
            ptr = NULL;
        }

        template <typename T>
        void NDeleteArray(T*& ptr)
        {
            if(ptr == NULL)
                return;
            NMemLocker locker;
            RemoveMemLog(MemTypeNewArray, ptr);
            delete[] ptr;
            ptr = NULL;
        }
    }
}
