#pragma once


#include "../base/BaseObj.h"

namespace nui
{
    namespace Parser
    {
        class NUI_CLASS NParser : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("parser"))
        public:
            virtual Base::NAutoPtr<Base::NBaseObj> Parse(LPCTSTR packFilePath) = 0;
        };
    }
}
