#pragma once


#include "../../NCursor.h"


namespace nui
{
    namespace Ui
    {
        namespace Gdi
        {
            class GdiCursor : public NCursor
            {
            public:
                GdiCursor(LPCTSTR cursorName);
                HCURSOR GetCursor() const;

            private:
                HCURSOR cursor_;
            };
        }
    }
}