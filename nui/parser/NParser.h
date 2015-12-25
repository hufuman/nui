#pragma once


#include "../base/BaseObj.h"

namespace nui
{
    namespace Parser
    {
        class NUI_INTF NParser : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("parser"))
        public:
            virtual Base::NAutoPtr<Base::NBaseObj> Parse(Base::NBaseObj* parentObj, LPCTSTR packFilePath) = 0;
            virtual nui::Base::NAutoPtr<nui::Data::NDataReader> FindStyleNode(LPCTSTR packFilePath) = 0;
        };
    }
}
