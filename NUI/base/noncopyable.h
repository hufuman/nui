#pragma once


namespace nui
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
            Noncopyable& operator =(const Noncopyable&);
        };
    }
}