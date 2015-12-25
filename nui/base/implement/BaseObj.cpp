#include "stdafx.h"
#include "../BaseObj.h"

namespace nui
{
    namespace Base
    {
        NBaseObj::NBaseObj()
        {
            refCount_ = 0;
        }

        NBaseObj::~NBaseObj()
        {}

        INT NBaseObj::AddRef()
        {
            return (++ refCount_);
        }

        INT NBaseObj::Release()
        {
            if((--refCount_) == 0)
            {
                NDeleteThis(this);
                return 0;
            }
            return refCount_;
        }

        INT NBaseObj::RefCount() const
        {
            return refCount_;
        }
    }
}
