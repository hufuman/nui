#pragma once


#include "../base/noncopyable.h"
#include "../data/NArrayT.h"

namespace nui
{
    namespace Ui
    {
        typedef FastDelegate1<int, void> NIdleHandler;
		typedef FastDelegate1<MSG*, BOOL> NMessageFilter;

        NUI_TEMPLATE template class NUI_CLASS Data::NArrayT<NIdleHandler>;
		NUI_TEMPLATE template class NUI_CLASS Data::NArrayT<NMessageFilter>;

        class NUI_CLASS NMsgLoop : public nui::Base::NBaseObj
        {
			DECLARE_REFLECTION(TEXT("nui"), TEXT("msgloop"))
		
		public:
            bool Loop();
            bool Loop(HWND window);
            void Stop();

            bool AddIdleHandler(NIdleHandler handler);
            bool RemoveIdleHandler(NIdleHandler handler);

			bool AddMessageFilter(NMessageFilter filter);
			bool RemoveMessageFilter(NMessageFilter filter);

        private:
            bool Loop(HWND window, bool useWindow);
            bool DoIdleHandler(int idleCount);
			bool DoMessageFilter(MSG* msg);

        private:
            MSG msg_;
            bool looping_;
            Data::NArrayT<NIdleHandler> idleHandlers_;
			Data::NArrayT<NMessageFilter> messageFilters_;
		};
    }
}
