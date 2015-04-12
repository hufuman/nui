#pragma once


#include "NRender.h"

namespace nui
{
    namespace Ui
    {
        class NRenderClip
        {
        public:
            NRenderClip(NRender* render, const nui::Base::NRect& rect1, const nui::Base::NRect& rect2)
            {
                Base::NRect tmpClipRect(rect1);
                tmpClipRect.Intersect(rect2);
                Init(render, tmpClipRect);
            }
            NRenderClip(NRender* render, const nui::Base::NRect& clipRect)
            {
                Init(render, clipRect);
            }
            ~NRenderClip()
            {
                holder_.Release();
            }

        protected:
            void Init(NRender* render, const nui::Base::NRect& clipRect)
            {
                if(render == NULL
                    || clipRect.Right <= clipRect.Left
                    || clipRect.Bottom <= clipRect.Top)
                {
                    return;
                }

                holder_ = render->ClipRect(clipRect);
            }

        protected:
            Base::NHolder holder_;
        };

    }
}
