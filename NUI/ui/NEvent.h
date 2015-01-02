#pragma once



#include "../base/BaseObj.h"
#include "../base/DataTypes.h"
#include "EventDef.h"

namespace NUI
{
    namespace UI
    {
        struct NEventData
        {
        public:
            EventType::Event    eventType;
            NUI::Base::NPoint   eventPos;
            DWORD               eventTime;
        };

        class NUI_INTF NEvent : public Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("event"))
        public:
            virtual bool OnEvent(NEventData* eventData) = 0;
        };
    }
}
