#pragma once



#include "../../base/noncopyable.h"
#include "../NWindowBase.h"


namespace NUI
{
    namespace UI
    {
        namespace Util
        {
            class WindowMap : protected NUI::Base::Noncopyable
            {
                static WindowMap instance_;
            public:
                WindowMap();
                ~WindowMap();

                static WindowMap& GetInst();

                bool AddWindow(HWND window, NWindowBase* windowObj);
                NWindowBase* GetWindow(HWND window);
                void RemoveWindow(HWND window);

            private:
                typedef std::map<HWND, NWindowBase*> WindowObjectMap;
                WindowObjectMap windowObjMap_;
            };
        }
    }
}
