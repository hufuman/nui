#pragma once


#include "../base/BaseObj.h"
#include "../ui/NFrame.h"

namespace nui
{
    namespace Parser
    {
        class NUI_CLASS NParser : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("parser"))
        public:
            virtual Base::NAutoPtr<Base::NBaseObj> Parse(Ui::NFrame* parentFrame, LPCTSTR packFilePath) = 0;
        };
    }
}
