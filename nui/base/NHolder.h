#pragma once

namespace nui
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

            bool IsEmpty() const;
            void Release();

            operator bool() const
            {
                return !IsEmpty();
            }

        private:
            HolderData* data_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
