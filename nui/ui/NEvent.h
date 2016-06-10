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

            template < typename TObjType >
            bool AddHandler(TObjType* handlerObj, bool (TObjType::*handler)(Base::NBaseObj*, NEventData*))
            {
                return AddHandler(MakeDelegate(handlerObj, handler));
            }

            bool AddHandler(NEventHandler handler);
            bool RemoveHandler(NEventHandler handler);

            // invoke all handlers untill one handler returns false
            //   return true only when no handler returns false
            bool Invoke(Base::NBaseObj* obj, NEventData* eventData);

            bool operator ()(Base::NBaseObj* obj, NEventData* eventData)
            {
                return Invoke(obj, eventData);
            }

            bool operator += (NEventHandler handler)
            {
                return AddHandler(handler);
            }

            bool operator -= (NEventHandler handler)
            {
                return RemoveHandler(handler);
            }

        public:
            void Clear();

        private:
            int embededLevel_;
            NEventNode* headNode_;
            NEventNode* tailNode_;
        };
    }
}