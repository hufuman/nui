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
            HICON GetBigIcon();
            HICON GetSmallIcon();

        public:
            HMODULE GetNUIModule() const;
            Base::NString GetAppPath() const;
            Base::NString GetAppFullName() const;

        private:
            HMODULE nuiModule_;
            Base::NString appPath_;
            Base::NString appFullName_;
            HICON   bigIcon_;
            HICON   smallIcon_;
        };
    }
}
