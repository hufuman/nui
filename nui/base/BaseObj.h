#pragma once

namespace nui
{
    namespace Base
    {
        /**
         * Base class of all nui classes, which provide refCount
         */
        class NUI_CLASS NBaseObj : protected Noncopyable
        {
        public:
            NBaseObj()
            {
                refCount_ = 0;
            }
            virtual ~NBaseObj(){}

            virtual INT AddRef()
            {
                return (++ refCount_);
            }

            virtual INT Release()
            {
                if((--refCount_) == 0)
                {
                    Base::NDeleteThis(this);
                    return 0;
                }
                return refCount_;
            }

            virtual INT RefCount() const
            {
                return refCount_;
            }

        private:
            INT refCount_;
        };

    }
}
