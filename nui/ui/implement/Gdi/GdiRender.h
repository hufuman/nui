#pragma once


#include "../../NRender.h"
#include "MemDC.h"

namespace nui
{
    namespace Ui
    {
        class GdiRender : public NRender
        {
        public:
            GdiRender();
            ~GdiRender();

            virtual bool Init(HDC hDc, const Base::NRect& rcPaint);
            virtual void DrawBack();

            virtual void DrawShape(const Base::NRect& rect, NShape* shape);
            virtual void DrawImage(NImage* image, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue);

        private:
            MemDC memDC_;
        };
    }
}
