#pragma once

#include "BaseObj.h"

namespace nui
{
    namespace Base
    {
        class NUI_INTF NCore : public NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("core"))
        public:
            NCore(){}
            virtual ~NCore(){}

            virtual bool InitCore(LPCTSTR packFilePath, LPCTSTR lang) = 0;
            virtual void DestroyCore() = 0;
        };
    }
}
