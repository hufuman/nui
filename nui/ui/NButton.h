#pragma once


#include "NRichFrame.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NButton : public NRichFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("button"))
        public:
            NButton();
            ~NButton();
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
