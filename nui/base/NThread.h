#pragma once


namespace nui
{
    namespace Base
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
        typedef FastDelegate1<bool&, void> ThreadImpl;

        class NUI_CLASS NThread : public Noncopyable
        {
        public:
            NThread();
            ~NThread();

        public:
            bool Start(ThreadImpl impl);
            bool StopAndWait();

        protected:
            static unsigned int _stdcall ThreadFunc(void*);

        private:
            bool stopping_;
            HANDLE threadHandle_;
            ThreadImpl threadImpl_;
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}