#pragma once


#include "../base/noncopyable.h"
#include "../base/NHolder.h"
#include "NShapeDraw.h"
#include "NResourceLoader.h"
#include "NUiBus.h"
#include "NText.h"
#include "NFont.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NRender : public nui::Base::NBaseObj
        {
        public:
            virtual bool Init(HDC hDc, const Base::NRect& rcPaint) = 0;
            virtual void DrawBack(bool layered) = 0;

            virtual void DrawShape(NShapeDraw* shape, const Base::NRect& rect) = 0;
            virtual void DrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue) = 0;
            virtual void DrawText(NText* text, NFont* font, const Base::NRect& rect) = 0;
            virtual void GetTextSize(NText* text, NFont* font, Base::NSize& rect) = 0;

            virtual Base::NHolder ClipRect(const nui::Base::NRect& rect) = 0;

            __inline void DrawLine(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShapeDraw> shape = loader->CreateShape(MemToolParam);
                shape->SetStyle(NShapeDraw::Line)->SetBorderWidth(borderWidth)->SetBorderColor(clrBorder);
                DrawShape(shape, rect);
            }

            __inline void DrawRoundRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShapeDraw> shape = loader->CreateShape(MemToolParam);
                shape->SetStyle(NShapeDraw::RoundRect)->SetBorderWidth(borderWidth)->SetBorderColor(clrBorder);
                DrawShape(shape, rect);
            }

            __inline void DrawRoundRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder, ArgbColor clrFill)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShapeDraw> shape = loader->CreateShape(MemToolParam);
                shape->SetStyle(NShapeDraw::RoundRect)->SetBorderWidth(borderWidth)->SetBorderColor(clrBorder)->SetFillColor(clrFill);
                DrawShape(shape, rect);
            }

            __inline void FillRoundRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrFill)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShapeDraw> shape = loader->CreateShape(MemToolParam);
                shape->SetStyle(NShapeDraw::RoundRect)->SetFillColor(clrFill)->SetBorderWidth(borderWidth);
                DrawShape(shape, rect);
            }

            __inline void DrawRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShapeDraw> shape = loader->CreateShape(MemToolParam);
                shape->SetStyle(NShapeDraw::Rect)->SetBorderWidth(borderWidth)->SetBorderColor(clrBorder);
                DrawShape(shape, rect);
            }

            __inline void FillRectangle(const Base::NRect& rect, ArgbColor clrFill)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShapeDraw> shape = loader->CreateShape(MemToolParam);
                shape->SetStyle(NShapeDraw::Rect)->SetFillColor(clrFill);
                DrawShape(shape, rect);
            }

            __inline void DrawRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder, ArgbColor clrFill)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShapeDraw> shape = loader->CreateShape(MemToolParam);
                shape->SetStyle(NShapeDraw::Rect)->SetBorderWidth(borderWidth)->SetBorderColor(clrBorder)->SetFillColor(clrFill);
                DrawShape(shape, rect);
            }

            __inline void DrawImage(NImageDraw* image, int horzIndex, int vertIndex, const Base::NRect& dstRect, int frameIndex)
            {
                DrawImage(image, horzIndex, vertIndex, dstRect.Left, dstRect.Top, dstRect.Width(), dstRect.Height(), frameIndex);
            }

            __inline void DrawImage(NImageDraw* image, int horzIndex, int vertIndex, int dstX, int dstY, int frameIndex)
            {
                Base::NSize size = image->GetPreferSize();
                DrawImage(image, horzIndex, vertIndex, dstX, dstY, size.Width, size.Height, frameIndex);
            }

            __inline void DrawImage(NImageDraw* image, int horzIndex, int vertIndex, int dstX, int dstY, int dstWidth, int dstHeight, int frameIndex)
            {
                Base::NSize size = image->GetPreferSize();
                int horzCount, vertCount;
                image->GetCount(horzCount, vertCount);

                horzIndex = horzIndex % horzCount;
                horzIndex = (horzIndex < 0) ? 0 : horzIndex;

                vertIndex = vertIndex % vertCount;
                vertIndex = (vertIndex < 0) ? 0 : vertIndex;

                int srcX = size.Width * horzIndex;
                int srcY = size.Height * vertIndex;
                int srcWidth = size.Width;
                int srcHeight = size.Height;

                DrawImage(image, frameIndex, srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, 255);
            }
        };
    }
}
