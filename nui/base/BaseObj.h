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
                    NDeleteThis(this);
                    return 0;
                }
                return refCount_;
            }

            virtual INT RefCount() const
            {
                return refCount_;
            }

            virtual bool SetAttr(LPCTSTR attrName, LPCTSTR attrValue)
            {
                UNREFERENCED_PARAMETER(attrName);
                UNREFERENCED_PARAMETER(attrValue);
                return false;
            }

        private:
            INT refCount_;
        };

    }
}
