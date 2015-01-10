#pragma once


namespace nui
{
    namespace Base
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
        typedef FastDelegate1<void*, unsigned int> ThreadImpl;

        class NUI_CLASS NThread : public Noncopyable
        {
        public:
            NThread();
            ~NThread();

        public:
            bool Start(ThreadImpl impl);
            bool Stop(DWORD timeout, bool force);
            bool Wait(DWORD timeout);

        protected:
            static unsigned int _stdcall ThreadFunc(void*);

        private:
            HANDLE threadHandle_;
            ThreadImpl threadImpl_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}