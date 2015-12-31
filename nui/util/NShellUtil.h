#pragma once


#include "../data/NArrayT.h"
#include "../ui/NWindow.h"

namespace nui
{
    namespace Util
    {
        namespace Shell
        {
            // filter: Exe File|*.exe|All Files|*.*||
            NUI_API bool BrowseForFile(Base::NString& result, HWND hWnd, BOOL openFile, LPCTSTR filter, LPCTSTR initDir = NULL, LPCTSTR title = NULL, DWORD flags = OFN_NODEREFERENCELINKS);
            NUI_API Data::NArrayT<Base::NString> BrowseForMultiFiles(HWND hWnd, BOOL openFile, LPCTSTR filter, LPCTSTR initDir = NULL, LPCTSTR szTitle = NULL, DWORD dwFlags = OFN_NODEREFERENCELINKS);
            NUI_API BOOL FilterWindowMessage(UINT message, DWORD dwValue);
            NUI_API Base::NRect GetWorkareaRect();
            NUI_API BOOL IsKeyPressed(int virKey);
            NUI_API Base::NPoint GetCurrentPos(Ui::NWindow* window = NULL);
        }
    }
}
