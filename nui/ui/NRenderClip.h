#pragma once


#include "NRender.h"

namespace nui
{
    namespace Ui
    {
        class NRenderClip
        {
        public:
            NRenderClip(NRender* render, HRGN clipRgn, const nui::Base::NRect& rect)
            {
                HRGN tempRgn = ::CreateRectRgn(rect.Left, rect.Top, rect.Right, rect.Bottom);
                int result = ::CombineRgn(tempRgn, tempRgn, clipRgn, RGN_AND);
                if(result != ERROR && result != NULLREGION)
                {
                    holder_ = render->ClipRgn(tempRgn);
                }
            }
            ~NRenderClip()
            {
                holder_.Release();
            }

            bool IsEmpty()
            {
                return holder_.IsEmpty();
            }

        protected:
            Base::NHolder holder_;
        };

    }
}
