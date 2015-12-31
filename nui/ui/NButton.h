#pragma once


#include "NFrame.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NButton : public NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("button"))
        public:
            NButton();
            ~NButton();

        protected:
            virtual void OnCreate();
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
