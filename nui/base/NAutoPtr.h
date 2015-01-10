#pragma once


namespace nui
{
    namespace Base
    {
        template <typename T>
        class NAutoPtr
        {
        public:
            NAutoPtr() : ptr_(0)
            {}

            NAutoPtr(NAutoPtr& p)
            {
                ptr_ = p.Detach();
            }

            NAutoPtr(T* p)
            {
                ptr_ = p;
                if(ptr_)
                    ptr_->AddRef();
            }

            NAutoPtr(int nullPtr)
            {
                NAssertError(nullPtr == 0, _T("only null acceptable"));
                ptr_ = NULL;
            }

            ~NAutoPtr()
            {
                NSafeRelease(ptr_);
            }

            NAutoPtr& operator = (NAutoPtr& p)
            {
                NSafeRelease(ptr_);
                if(&p != this)
                    ptr_ = p.Detach();
                return (*this);
            }

            NAutoPtr& operator = (int nullPtr)
            {
                NAssertError(nullPtr == 0, _T("only null acceptable"));
                NSafeRelease(ptr_);
                ptr_ = NULL;
                return (*this);
            }

            NAutoPtr& operator = (T* p)
            {
                NSafeRelease(ptr_);
                ptr_ = p;
                if(ptr_)
                    ptr_->AddRef();
                return *this;
            }

            operator T*() const
            {
                return ptr_;
            }

            T* operator -> () const
            {
                NAssertError(ptr_ != NULL, _T("Null Ptr"));
                return ptr_;
            }

            T* Detach()
            {
                T* p = ptr_;
                ptr_ = NULL;
                return p;
            }

        protected:
            T* ptr_;
        };
    }
}