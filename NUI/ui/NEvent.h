#pragma once



#include "../base/BaseObj.h"
#include "../base/DataTypes.h"
#include "EventDef.h"

namespace nui
{
    namespace UI
    {
        struct NEventData
        {
        public:
            EventType::Event    eventType;
            nui::Base::NPoint   eventPos;
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
