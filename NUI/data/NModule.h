#pragma once


namespace nui
{
    namespace Data
    {
        class NUI_CLASS NModule : Base::Noncopyable
        {
            NModule();

            static NModule instance_;
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
