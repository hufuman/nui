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

            bool Init(HMODULE nuiModule, LPCTSTR iconFile = NULL);
            bool IsValid() const;
            HICON GetBigIcon();
            HICON GetSmallIcon();
            DWORD GetMainThreadId() const;

        public:
            HMODULE GetNUIModule() const;
            Base::NString GetAppPath() const;
            Base::NString GetAppFullName() const;

        private:
            HMODULE nuiModule_;
            Base::NString appPath_;
            Base::NString appFullName_;
			Base::NString appIconFile;
            HICON   bigIcon_;
            HICON   smallIcon_;
            DWORD   mainThreadId_;
        };
    }
}
