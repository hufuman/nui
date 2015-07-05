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

            virtual void DrawShape(NShapeDraw* shape, const Base::NRect& rect);
            virtual void DrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue);
            virtual void DrawText(NText* text, NFont* font, const Base::NRect& rect);
            virtual void GetTextSize(NText* text, NFont* font, Base::NSize& rect);
            virtual void AddDrawRegion(const Base::NRect& rcRegion);

            virtual Base::NHolder ClipRgn(HRGN clipRgn);

            MemDC& GetMemDC();

        protected:
            void RestoreRgn(void* data);

        protected:
            void FillRectImpl(HDC hDc, const Base::NRect& rect, ArgbColor fillColor);
            void DrawRectImpl(HDC hDc, const Base::NRect& rect, int width, ArgbColor borderColor);
            void DrawAndFillRectImpl(HDC hDc, const Base::NRect& rect, int width, ArgbColor borderColor, ArgbColor fillColor);

            void StretchDrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue);
            void TileDrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue);
            void NineStretchDrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue);

        private:
            HDC orgDc_;
            MemDC memDC_;
        };
    }
}
