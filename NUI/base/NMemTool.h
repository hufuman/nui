#pragma once


namespace NUI
{
    namespace Base
    {
        __inline void* NMalloc(int size)
        {
            return malloc(size);
        }

        __inline void NFree(void*& ptr)
        {
            free(ptr);
            ptr = NULL;
        }

        template <typename T>
        T* NNew()
        {
            return new T();
        }

        template <typename T>
        T* NNewArray(int count)
        {
            return new T[count];
        }

        template <typename T>
        void NDeleteThis(T* ptr)
        {
            delete ptr;
        }

        template <typename T>
        void NDelete(T*& ptr)
        {
            delete ptr;
            ptr = NULL;
        }

        template <typename T>
        void NDeleteArray(T*& ptr)
        {
            delete[] ptr;
            ptr = NULL;
        }
    }
}
