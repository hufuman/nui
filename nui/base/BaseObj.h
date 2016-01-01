#pragma once

namespace nui
{
    namespace Base
    {
#define NSafeRelease(ptr)       for(;;){if(ptr) ptr->Release(); ptr = NULL;break;}
#define NEnsureRelease(ptr)     for(;ptr;){if(ptr->Release() == 0) {ptr = NULL;}break;}

#define NDelayedRelease(ptr)    for(;;){if(ptr) NDelayedReleaseImpl(ptr); ptr = NULL;break;}

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

        NUI_API void NDelayedReleaseImpl(void* ptr);

    }
}
