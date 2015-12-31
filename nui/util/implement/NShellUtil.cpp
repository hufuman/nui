#include "stdafx.h"

#include "../../Base/NString.h"
#include "../NShellUtil.h"

namespace
{
    bool BrowserForFile(OUT LPTSTR szResult, HWND hWnd, DWORD dwMaxResultLen, BOOL openFile, LPCTSTR filter, LPCTSTR initDir, LPCTSTR szTitle, DWORD dwFlags /* = OFN_NODEREFERENCELINKS */)
    {
        OPENFILENAME ofn = {0};
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hWnd;

        ofn.lpstrFile = szResult;
        ofn.nMaxFile = dwMaxResultLen;

        TCHAR tmpFilter[1024];
        _tcsncpy(tmpFilter, filter, 1024);
        TCHAR* p = tmpFilter;
        while(*p)
        {
            if(*p == _T('|'))
                *p = 0;
            ++ p;
        }
        *p = 0;

        UNREFERENCED_PARAMETER(szTitle);
        UNREFERENCED_PARAMETER(initDir);
        ofn.lpstrFilter = tmpFilter;
        ofn.lpstrTitle = szTitle;
        ofn.lpstrInitialDir = initDir;
        ofn.Flags = OFN_NOCHANGEDIR | OFN_ENABLESIZING | OFN_EXPLORER | OFN_OVERWRITEPROMPT | dwFlags;
        if(openFile)
        {
            ofn.Flags |= (OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST);
        }

        BOOL bResult = FALSE;
        if(openFile)
        {
            bResult = GetOpenFileName(&ofn);
        }
        else
        {
            bResult = GetSaveFileName(&ofn);
        }

        return !!bResult;
    }
}

namespace nui
{
    namespace Util
    {
        namespace Shell
        {
            NUI_API bool BrowseForFile(Base::NString& result, HWND hWnd, BOOL openFile, LPCTSTR filter, LPCTSTR initDir, LPCTSTR title, DWORD flags)
            {
                TCHAR buffer[4096] = {0};
                if(!result.IsEmpty())
                {
                    _tcsncpy(buffer, result.GetData(), _countof(buffer));
                }
                if(!BrowserForFile(buffer, hWnd, _countof(buffer), openFile, filter, initDir, title, flags))
                {
                    return false;
                }
                result = buffer;
                return true;
            }

            NUI_API Data::NArrayT<Base::NString> BrowseForMultiFiles(HWND hWnd, BOOL openFile, LPCTSTR filter, LPCTSTR initDir, LPCTSTR szTitle, DWORD dwFlags)
            {
                TCHAR szFilePath[4096] = {0};
                bool bResult = BrowserForFile(szFilePath, hWnd, 4096, openFile, filter, initDir, szTitle, dwFlags | OFN_ALLOWMULTISELECT);

                Data::NArrayT<Base::NString> arrResult;

                if(bResult)
                {
                    Base::NString strPath = szFilePath;
                    Base::NString strFilePath, strFileName;
                    size_t nLen = strPath.GetLength();

                    TCHAR* pFileName = szFilePath + nLen + 1;

                    if(pFileName[0] == 0)
                    {
                        // Only one file selected
                        strFileName = pFileName;
                        strFilePath = strPath + pFileName;
                        arrResult.AddItem(strFilePath);
                        return arrResult;
                    }

                    strPath += _T("\\");
                    while(pFileName != NULL && pFileName[0] != 0)
                    {
                        strFileName = pFileName;
                        strFilePath = strPath + pFileName;
                        arrResult.AddItem(strFilePath);
                        nLen += strFileName.GetLength() + 1;
                        pFileName = szFilePath + nLen + 1;
                    }
                }

                return arrResult;
            }

            NUI_API BOOL FilterWindowMessage(UINT message, DWORD dwValue)
            {
                typedef BOOL (WINAPI FAR *ChangeWindowMessageFilter_PROC)(UINT,DWORD);
                ChangeWindowMessageFilter_PROC pfnChangeWindowMessageFilter;
                pfnChangeWindowMessageFilter = (ChangeWindowMessageFilter_PROC)::GetProcAddress (::GetModuleHandle(_T("USER32")), "ChangeWindowMessageFilter");
                if(pfnChangeWindowMessageFilter == NULL)
                    return FALSE;
                return pfnChangeWindowMessageFilter(message, dwValue);
            }

            NUI_API Base::NRect GetWorkareaRect()
            {
                Base::NRect rcWorkarea;
                ::SystemParametersInfo(SPI_GETWORKAREA, 0, rcWorkarea, 0);
                return rcWorkarea;
            }

            NUI_API BOOL IsKeyPressed(int virKey)
            {
                return ((::GetKeyState(virKey) & 0x80000000) == 0x80000000);
            }

            NUI_API Base::NPoint GetCurrentPos(Ui::NWindow* window)
            {
                POINT pt;
                ::GetCursorPos(&pt);
                Base::NPoint point(pt.x, pt.y);
                if(window)
                {
                    Base::NRect rcWnd;
                    ::GetClientRect(window->GetNative(), rcWnd);
                    ::ClientToScreen(window->GetNative(), reinterpret_cast<LPPOINT>(&rcWnd));
                    ::ClientToScreen(window->GetNative(), reinterpret_cast<LPPOINT>(&rcWnd) + 1);
                    point.X -= rcWnd.Left;
                    point.Y -= rcWnd.Top;
                }
                return point;
            }
        }
    }
}
