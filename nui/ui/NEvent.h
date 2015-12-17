#pragma once

#include "../base/BaseObj.h"

namespace nui
{
    namespace Ui
    {
        class NUI_CLASS NEventData
        {
        };

        typedef fastdelegate::FastDelegate2<Base::NBaseObj*, NEventData*, bool> NEventHandler;

        class NUI_CLASS NEvent
        {
            class NEventNode
            {
            public:
                NEventNode* next;
                NEventHandler handler;
                bool valid;
            };

        public:
            NEvent();
            virtual ~NEvent();

            bool AddHandler(NEventHandler handler);
            bool RemoveHandler(NEventHandler handler);
            bool Invoke(Base::NBaseObj* obj, NEventData* eventData);

        public:
            void Clear();

        private:
            int embededLevel_;
            NEventNode* headNode_;
            NEventNode* tailNode_;
        };
    }
}