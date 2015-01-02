#include "stdafx.h"
#include "WindowMap.h"



namespace NUI
{
    namespace UI
    {
        namespace Util
        {
            WindowMap WindowMap::instance_;

            WindowMap::WindowMap()
            {}

            WindowMap::~WindowMap()
            {
                NAssertError(windowObjMap_.empty(), _T("There is window not removed"));
            }

            WindowMap& WindowMap::GetInst()
            {
                return instance_;
            }

            bool WindowMap::AddWindow(HWND window, NWindowBase* windowObj)
            {
                NAssertError(windowObjMap_.find(window) == windowObjMap_.end(), _T("Window already exists in WindowMap"));
                std::pair<WindowObjectMap::iterator, bool> result = windowObjMap_.insert(std::make_pair(window, windowObj));
                return result.second;
            }

            NWindowBase* WindowMap::GetWindow(HWND window)
            {
                WindowObjectMap::iterator ite = windowObjMap_.find(window);
                if(ite == windowObjMap_.end())
                    return NULL;
                return ite->second;
            }

            void WindowMap::RemoveWindow(HWND window)
            {
                WindowObjectMap::iterator ite = windowObjMap_.find(window);
                if(ite != windowObjMap_.end())
                    windowObjMap_.erase(ite);
            }
        }
    }
}
