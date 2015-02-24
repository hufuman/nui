#pragma once


#include "../base/noncopyable.h"
#include "../base/NHolder.h"
#include "NShape.h"
#include "NResourceLoader.h"
#include "NUiBus.h"
#include "NText.h"

namespace nui
{
    namespace Ui
    {
        class NUI_INTF NRender : public nui::Base::NBaseObj
        {
        public:
            virtual bool Init(HDC hDc, const Base::NRect& rcPaint) = 0;
            virtual void DrawBack() = 0;

            virtual void DrawShape(const Base::NRect& rect, NShape* shape) = 0;
            virtual void DrawImage(NImage* image, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue) = 0;
            virtual void DrawText(NText* text, const Base::NRect& rect) = 0;
            virtual void GetTextSize(NText* text, Base::NRect& rect) = 0;

            virtual bool IsRectVisible(const Base::NRect& rect) = 0;
            virtual void OffsetRender(int xOffset, int yOffset) = 0;

            __inline void DrawLine(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShape> shape = loader->CreateShape();
                shape->SetStyle(NShape::Line).SetBorderWidth(borderWidth).SetBorderColor(clrBorder);
                DrawShape(rect, shape);
            }

            __inline void DrawRoundRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShape> shape = loader->CreateShape();
                shape->SetStyle(NShape::RoundRect).SetBorderWidth(borderWidth).SetBorderColor(clrBorder);
                DrawShape(rect, shape);
            }

            __inline void DrawRoundRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder, ArgbColor clrFill)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShape> shape = loader->CreateShape();
                shape->SetStyle(NShape::RoundRect).SetBorderWidth(borderWidth).SetBorderColor(clrBorder).SetFillColor(clrFill);
                DrawShape(rect, shape);
            }

            __inline void FillRoundRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrFill)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShape> shape = loader->CreateShape();
                shape->SetStyle(NShape::RoundRect).SetFillColor(clrFill).SetBorderWidth(borderWidth);
                DrawShape(rect, shape);
            }

            __inline void DrawRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShape> shape = loader->CreateShape();
                shape->SetStyle(NShape::Rect).SetBorderWidth(borderWidth).SetBorderColor(clrBorder);
                DrawShape(rect, shape);
            }

            __inline void FillRectangle(const Base::NRect& rect, ArgbColor clrFill)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShape> shape = loader->CreateShape();
                shape->SetStyle(NShape::Rect).SetFillColor(clrFill);
                DrawShape(rect, shape);
            }

            __inline void DrawRectangle(const Base::NRect& rect, int borderWidth, ArgbColor clrBorder, ArgbColor clrFill)
            {
                Ui::NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
                Base::NAutoPtr<NShape> shape = loader->CreateShape();
                shape->SetStyle(NShape::Rect).SetBorderWidth(borderWidth).SetBorderColor(clrBorder).SetFillColor(clrFill);
                DrawShape(rect, shape);
            }

            __inline void DrawImage(NImage* image, const Base::NRect& dstRect)
            {
                DrawImage(image, dstRect.Left, dstRect.Top, dstRect.Width(), dstRect.Height());
            }

            __inline void DrawImage(NImage* image, int dstX, int dstY)
            {
                Base::NSize size = image->GetSize();
                DrawImage(image, dstX, dstY, size.Width, size.Height);
            }

            __inline void DrawImage(NImage* image, int dstX, int dstY, int dstWidth, int dstHeight)
            {
                Base::NSize size = image->GetSize();
                int horzCount, vertCount;
                image->GetCount(horzCount, vertCount);
                int horzIndex, vertIndex;
                image->GetIndex(horzIndex, vertIndex);
                int srcX = size.Width * horzIndex / horzCount;
                int srcY = size.Height * vertIndex / vertCount;
                int srcWidth = size.Width / horzCount;
                int srcHeight = size.Height / vertCount;
                DrawImage(image, srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, 255);
            }
        };
    }
}
