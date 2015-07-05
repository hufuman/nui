#include "stdafx.h"
#include "../NHolder.h"



namespace nui
{
    namespace Base
    {
        NHolder::NHolder()
        {
            data_ = NULL;
        }
        NHolder::NHolder(void* data, FastDelegate1<void*> func)
        {
            data_ = NNew(HolderData);
            data_->data_ = data;
            data_->func_ = func;
            data_->refCount_ = 1;
        }

        NHolder::NHolder(const NHolder& holder)
        {
            data_ = holder.data_;
            ++ data_->refCount_;
        }

        NHolder& NHolder::operator = (const NHolder& holder)
        {
            if(&holder != this)
            {
                Release();
                data_ = holder.data_;
                ++ data_->refCount_;
            }
            return (*this);
        }

        bool NHolder::IsEmpty() const
        {
            return data_ == NULL;
        }

        void NHolder::Release()
        {
            if(data_ != NULL)
            {
                -- data_->refCount_;
                if(data_->refCount_ == 0)
                {
                    data_->func_(data_->data_);
                    NDelete(data_);
                }
                data_ = NULL;
            }
        }

        NHolder::~NHolder()
        {
            Release();
        }
    }
}

