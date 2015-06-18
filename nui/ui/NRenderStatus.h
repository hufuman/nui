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
            void DrawStatus(NRender* render);

        protected:
            DWORD lastTime_;
            Base::NInstPtr<NText> text_;
        };
    }
}
