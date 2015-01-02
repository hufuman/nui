#pragma once


#include "../base/noncopyable.h"
#include "../base/NHolder.h"

namespace NUI
{
    namespace UI
    {
        typedef FastDelegate0<> NTimerCallback;

        class NUI_CLASS NTimerSrv : public NUI::Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("timersrv"))
        public:
            virtual Base::NHolder startTimer(int elapse, NTimerCallback callback) = 0;
            virtual Base::NHolder startOnceTimer(int elapse, NTimerCallback callback) = 0;
        };
    }
}
