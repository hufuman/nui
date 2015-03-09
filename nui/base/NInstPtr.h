#pragma once


namespace nui
{
    namespace Base
    {
        class NReflect;
        template <typename T>
        class NInstPtr
        {
        public:
            NInstPtr(LPCSTR filePath, int line) : ptr_(0)
            {
                Create(filePath, line);
            }

            NInstPtr(NInstPtr& p)
            {
                ptr_ = p.ptr_;
                if(ptr_)
                    ptr_->AddRef();
            }

            ~NInstPtr()
            {
                NSafeRelease(ptr_);
            }

            NInstPtr& operator = (NInstPtr& p)
            {
                NSafeRelease(ptr_);
                if(&p != this)
                {
                    ptr_ = p.ptr_;
                    if(ptr_)
                        ptr_->AddRef();
                }
                return (*this);
            }

            NInstPtr& operator = (int nullPtr)
            {
                NAssertError(nullPtr == 0, _T("only null acceptable"));
                NSafeRelease(ptr_);
                ptr_ = NULL;
                return (*this);
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

            T* Create(LPCSTR filePath, int line)
            {
                nui::Base::NReflect::GetInstance().Create(ptr_, filePath, line);
                if(ptr_)
                    ptr_->AddRef();
                NAssertError(ptr_ != NULL, _T("Failed to create obj"));
                return ptr_;
            }

            T* Detach()
            {
                T* p = ptr_;
                ptr_ = NULL;
                return p;
            }

            operator NAutoPtr<T>()
            {
                NAutoPtr<T> ptr(ptr_);
                return ptr;
            }

        protected:
            T* ptr_;
        };
    }
}