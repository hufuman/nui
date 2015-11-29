#include "stdafx.h"
#include "GdiCursor.h"

#include "../../NCursor.h"


namespace nui
{
    namespace Ui
    {
        namespace Gdi
        {
            GdiCursor::GdiCursor(LPCTSTR cursorName)
            {
                cursor_ = ::LoadCursor(NULL, cursorName);
            }

            HCURSOR GdiCursor::GetCursor() const
            {
                return cursor_;
            }

            GdiCursor g_CursorList[] = 
            {
                // CursorArrow
                GdiCursor(MAKEINTRESOURCE(IDC_ARROW)),
                // CursorHand
                GdiCursor(MAKEINTRESOURCE(IDC_HAND)),
                // CursorUpDown
                GdiCursor(MAKEINTRESOURCE(IDC_SIZENS)),
                // CursorLeftRight
                GdiCursor(MAKEINTRESOURCE(IDC_SIZEWE)),
                // CursorCross
                GdiCursor(MAKEINTRESOURCE(IDC_SIZEALL)),
                // CursorSlash
                GdiCursor(MAKEINTRESOURCE(IDC_SIZENESW)),
                // CursorBackSlash
                GdiCursor(MAKEINTRESOURCE(IDC_SIZENWSE)),
            };
        }


        NCursor::~NCursor()
        {
            ;
        }

        NUI_API NCursor* GetCursorByType(NCursor::CursorType type)
        {
            int index = static_cast<int>(type);
            if(index >= 0 && index < _countof(Gdi::g_CursorList))
                return &Gdi::g_CursorList[index];
            return NULL;
        }
    }
}