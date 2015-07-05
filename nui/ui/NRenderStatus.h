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

            void BeforeDraw(HWND hWnd);
            void DrawStatus();

        protected:
            DWORD lastTime_;
            int index_;

            HWND hWnd_;
        };
    }
}
