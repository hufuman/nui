#pragma once

#include "BaseObj.h"

namespace NUI
{
    namespace Base
    {
        class NUI_INTF NCore : public NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("core"))
        public:
            NCore(){}
            virtual ~NCore(){}

            virtual bool InitCore(LPCTSTR szUIData, LPCTSTR szLang) = 0;
            virtual void DestroyCore() = 0;
        };
    }
}
