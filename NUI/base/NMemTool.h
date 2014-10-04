#pragma once


namespace NUI
{
    namespace Base
    {
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
        void NFreeThis(T* ptr)
        {
            delete ptr;
        }

        template <typename T>
        void NFree(T*& ptr)
        {
            delete ptr;
            ptr = NULL;
        }

        template <typename T>
        void NFreeArray(T*& ptr)
        {
            delete[] ptr;
            ptr = NULL;
        }
    }
}
