#pragma once


#include "../NTimerSrv.h"
#include "../NWindowBase.h"

namespace nui
{
    namespace Ui
    {
        struct TimerData
        {
            bool isOnceTimer;
            NTimerCallback callback;
        };

        class NTimerSrvImpl : public NTimerSrv
        {
        public:
            NTimerSrvImpl();
            ~NTimerSrvImpl();

        public:
            virtual Base::NHolder startTimer(int elapse, NTimerCallback callback);
            virtual Base::NHolder startOnceTimer(int elapse, NTimerCallback callback);

        protected:
            static void ReleaseTimer(void* data);
            UINT_PTR GetNextTimerId();
            Base::NHolder startTimerImpl(int elapse, NTimerCallback callback, bool isOnce);
            void stopTimer(UINT_PTR timerId);

            bool TimerWindowProc(NWindowBase* window, UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

        private:
            typedef std::map<UINT_PTR, TimerData> TimerMap;
            TimerMap timerMap_;
            UINT_PTR baseTimerId_;

            NWindowBase window_;
        };
    }
}
