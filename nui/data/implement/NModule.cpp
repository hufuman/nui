#include "stdafx.h"

#include "../NModule.h"
#include "../../base/NString.h"
#include "../../Util/NFileUtil.h"


namespace nui
{
    namespace Data
    {
        NModule NModule::instance_;

        NModule::NModule() : appIconFile(_T("icon.ico"))
        {
            nuiModule_ = ::GetModuleHandle(NULL);
            bigIcon_ = NULL;
            smallIcon_ = NULL;
            mainThreadId_ = 0;
        }

        NModule::~NModule()
        {
            nuiModule_ = NULL;
			if(bigIcon_)
				DestroyIcon(bigIcon_);
			if(smallIcon_)
				DestroyIcon(smallIcon_);
		}

        NModule& NModule::GetInst()
        {
            return instance_;
        }

        bool NModule::Init(HMODULE nuiModule, LPCTSTR iconFile)
        {
            if(nuiModule != NULL)
                nuiModule_ = nuiModule;
			if (iconFile != NULL)
				appIconFile = iconFile;

            TCHAR tempPath[1024];
            ::GetModuleFileName(NULL, tempPath, 1024);
            appFullName_ = tempPath;
            appPath_ = Util::File::GetParentFolder(tempPath);
            mainThreadId_ = ::GetCurrentThreadId();
            return true;
        }

        bool NModule::IsValid() const
        {
            return (nuiModule_ != NULL);
        }

        HICON NModule::GetBigIcon()
        {
            if(bigIcon_ != NULL)
                return bigIcon_;
			HICON hIcon = (HICON)::LoadImage(nuiModule_, appPath_ + appIconFile, IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_LOADFROMFILE);
			if (hIcon != NULL)
				bigIcon_ = hIcon;
			else
				::ExtractIconEx(appFullName_, 0, &bigIcon_, &smallIcon_, 1);
            return bigIcon_;
        }

        HICON NModule::GetSmallIcon()
        {
            if(smallIcon_ != NULL)
                return smallIcon_;
			HICON hIcon = (HICON)::LoadImage(nuiModule_, appPath_ + appIconFile, IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_LOADFROMFILE);
			if (hIcon != NULL)
				smallIcon_ = hIcon;
			else
				::ExtractIconEx(GetAppPath(), 0, &bigIcon_, &smallIcon_, 1);
            return smallIcon_;
        }

        HMODULE NModule::GetNUIModule() const
        {
            return nuiModule_;
        }

        Base::NString NModule::GetAppPath() const
        {
            return appPath_;
        }

        Base::NString NModule::GetAppFullName() const
        {
            return appFullName_;
        }

        DWORD NModule::GetMainThreadId() const
        {
            return mainThreadId_;
        }
    }
};
