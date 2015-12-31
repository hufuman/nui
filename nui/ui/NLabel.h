#pragma once


#include "NFrame.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NLabel : public NFrame
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("label"))
        public:
            NLabel();
            ~NLabel();

        protected:
            virtual void OnCreate();
        };
        END_USE_UNEXPORT_TEMPLATE()
    }
}
