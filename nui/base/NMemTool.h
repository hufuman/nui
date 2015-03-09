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

#define NMalloc(size) nui::Base::NMallocImpl(size, MemToolParam)
        __inline void* NMallocImpl(int size, LPCSTR szFilePath, int nLine)
        {
            NMemLocker locker;
            void* p = malloc(size);
            AddMemLog(MemTypeMalloc, p, szFilePath, nLine, "malloc", size, 1);
            return p;
        }

#define NFree(ptr)  for(;;){nui::Base::NFreeImpl(reinterpret_cast<void*>(ptr)); ptr=NULL;break;}
        __inline void NFreeImpl(void* ptr)
        {
            if(ptr == NULL)
                return;
            NMemLocker locker;
            RemoveMemLog(MemTypeMalloc, ptr);
            free(ptr);
        }

#define NNew(type, ...) nui::Base::NNewImpl<type>(nui::Base::MemTypeNew, new type(__VA_ARGS__), 1, MemToolParam)
#define NNewEx(type, file, line, ...) nui::Base::NNewImpl<type>(nui::Base::MemTypeNew, new type(__VA_ARGS__), 1, file, line)
#define NNewArray(type, count) nui::Base::NNewImpl<type>(nui::Base::MemTypeNewArray, new type[count], count, MemToolParam)
        template <typename T>
        T* NNewImpl(NuiMemType memType, T* p, int count, LPCSTR szFilePath, int nLine)
        {
            NMemLocker locker;
            AddMemLog(memType, p, szFilePath, nLine, typeid(T).name(), sizeof(T), count);
            return p;
        }

#define NDeleteThis(ptr) nui::Base::NDeleteThisImpl(ptr)
        template <typename T>
        void NDeleteThisImpl(T* ptr)
        {
            if(ptr == NULL)
                return;
            NMemLocker locker;
            RemoveMemLog(MemTypeNew, ptr);
            delete ptr;
        }

#define NDelete(ptr) nui::Base::NDeleteImpl(ptr)
        template <typename T>
        void NDeleteImpl(T*& ptr)
        {
            if(ptr == NULL)
                return;
            NMemLocker locker;
            RemoveMemLog(MemTypeNew, ptr);
            delete ptr;
            ptr = NULL;
        }

#define NDeleteArray(ptr) nui::Base::NDeleteArrayImpl(ptr)
        template <typename T>
        void NDeleteArrayImpl(T*& ptr)
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
