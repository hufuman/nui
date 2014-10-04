#pragma once


namespace NUI
{
    namespace Base
    {
        class NUI_CLASS Noncopyable
        {
        public:
            Noncopyable(){}
            virtual ~Noncopyable(){}
        private:
            Noncopyable(const Noncopyable&){}
        };
    }
}