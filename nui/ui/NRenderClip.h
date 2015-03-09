#pragma once


#include "NRender.h"

namespace nui
{
    namespace Ui
    {
        class NRenderClip
        {
        public:
            NRenderClip(NRender* render, const nui::Base::NRect& clipRect)
            {
                if(render == NULL
                    || clipRect.Right <= clipRect.Left
                    || clipRect.Bottom <= clipRect.Top)
                {
                    return;
                }

                holder_ = render->ClipRect(clipRect);
            }
            ~NRenderClip()
            {
                holder_.Release();
            }

        protected:
            Base::NHolder holder_;
        };

    }
}
