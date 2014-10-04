#pragma once


namespace NUI
{
    namespace Data
    {
        class NUI_CLASS NModule : Base::Noncopyable
        {
            NModule();

            static NModule _instance;
        public:
            ~NModule();
            static NModule& GetInst();

            bool Init(HMODULE nuiModule);
            bool IsValid() const;

        public:
            HMODULE GetNUIModule() const;
            Base::NString GetAppPath() const;

        private:
            HMODULE nuiModule_;
            Base::NString appPath_;
        };
    }
}
