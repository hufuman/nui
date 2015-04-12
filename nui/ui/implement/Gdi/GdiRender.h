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
            virtual void DrawBack(bool layered);

            virtual void DrawShape(NShape* shape, const Base::NRect& rect);
            virtual void DrawImage(NImage* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue);
            virtual void DrawText(NText* text, NFont* font, const Base::NRect& rect);
            virtual void GetTextSize(NText* text, NFont* font, Base::NSize& rect);
            virtual Base::NHolder ClipRect(const nui::Base::NRect& rect);

            MemDC& GetMemDC();

        protected:
            void RestoreRgn(void* data);

        protected:
            void FillRectImpl(HDC hDc, const Base::NRect& rect, ArgbColor fillColor);
            void DrawRectImpl(HDC hDc, const Base::NRect& rect, int width, ArgbColor borderColor);
            void DrawAndFillRectImpl(HDC hDc, const Base::NRect& rect, int width, ArgbColor borderColor, ArgbColor fillColor);

        private:
            HDC orgDc_;
            MemDC memDC_;
        };
    }
}
