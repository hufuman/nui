#pragma once

#include "../base/BaseObj.h"

namespace nui
{
    namespace Data
    {
        class NUI_INTF NStringBundle : public nui::Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("stringBundle"))
        public:
            virtual bool InitStringBundle(LPCTSTR stringLang) = 0;
            virtual const nui::Base::NString& GetString(const nui::Base::NString& name) = 0;
        };
    }
}
