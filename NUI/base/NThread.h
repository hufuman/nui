#pragma once


namespace NUI
{
    namespace Base
    {
#pragma warning(push)
#pragma warning(disable: 4251)

        typedef std::tr1::function<unsigned int (void*)> ThreadImpl;

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

#pragma warning(pop)

    }
}