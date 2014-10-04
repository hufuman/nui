#include "stdafx.h"

#include "../../base/NString.h"
#include "../NModule.h"

#include "../../Util/NFileUtil.h"


namespace NUI
{
    namespace Data
    {
        NModule NModule::instance_;

        NModule::NModule()
        {
            nuiModule_ = NULL;
        }

        NModule::~NModule()
        {
            nuiModule_ = NULL;
        }

        NModule& NModule::GetInst()
        {
            return instance_;
        }

        bool NModule::Init(HMODULE nuiModule)
        {
            nuiModule_ = nuiModule;

            TCHAR tempPath[1024];
            GetModuleFileName(NULL, tempPath, 1024);
            appPath_ = Util::File::GetParentFolder(tempPath);

            return true;
        }

        bool NModule::IsValid() const
        {
            return (nuiModule_ != NULL);
        }

        HMODULE NModule::GetNUIModule() const
        {
            return nuiModule_;
        }

        Base::NString NModule::GetAppPath() const
        {
            return appPath_;
        }
    }
};
