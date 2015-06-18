#pragma once

#include "NRender.h"

namespace nui
{
    namespace Ui
    {
        class NUI_CLASS NRenderStatus : public Base::NBaseObj
        {
        public:
            NRenderStatus();
            ~NRenderStatus();

            void BeforeDraw();
            void DrawStatus(NRender* render, const Base::NRect& rcClient);

        protected:
            DWORD lastTime_;

            BEGIN_USE_UNEXPORT_TEMPLATE()
                Base::NAutoPtr<NText> text_;
            END_USE_UNEXPORT_TEMPLATE()
        };
    }
}
