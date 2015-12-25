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
            NBaseObj();
            virtual ~NBaseObj();

            virtual INT AddRef();
            virtual INT Release();
            virtual INT RefCount() const;

        private:
            INT refCount_;
        };

    }
}
