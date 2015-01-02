#pragma once

namespace NUI
{
    namespace Base
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
        class NUI_CLASS NHolder : public Base::Noncopyable
        {
            struct HolderData
            {
                void*   data_;
                int     refCount_;
                FastDelegate1<void*>    func_;
            };
        public:
            NHolder();
            NHolder(void* data, FastDelegate1<void*> func);
            NHolder(const NHolder& holder);
            NHolder& operator = (const NHolder& holder);
            ~NHolder();

            void Release();

        private:
            HolderData* data_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
