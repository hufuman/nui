#include "stdafx.h"
#include "GdiRender.h"

#include "ImageDC.h"
#include "AlphaDC.h"
#include "GdiUtil.h"
#include "GdiShape.h"
#include "GdiImage.h"
#include "GdiText.h"
#include "GdiUtil.h"

namespace nui
{
    namespace Ui
    {
        GdiRender::GdiRender()
        {}

        GdiRender::~GdiRender()
        {}

        bool GdiRender::Init(HDC hDc, const Base::NRect& rcPaint)
        {
            memDC_.Init(hDc, rcPaint, 255);
            ::SetBkMode(memDC_, TRANSPARENT);
            return true;
        }

        void GdiRender::DrawBack()
        {
            memDC_.DrawBack();
            memDC_.Destroy();
        }

        void GdiRender::DrawShape(const Base::NRect& rect, NShape* shape)
        {
            GdiShape* gdiShape = dynamic_cast<GdiShape*>(shape);

            HGDIOBJ pen = gdiShape->GetPen();
            HGDIOBJ brush = gdiShape->GetBrush();
            if(pen == NULL && brush == NULL)
                return;

            BYTE penAlpha = gdiShape->GetBorderAlpha();
            BYTE brushAlpha = gdiShape->GetFillAlpha();

            if(penAlpha == brushAlpha)
            {
                CAlphaDC alphaDc;
                alphaDc.Init(memDC_, rect, false);

                Gdi::CGdiSelector penSelector(alphaDc, pen, false);
                Gdi::CGdiSelector brushSelector(alphaDc, brush, false);

                switch(shape->GetStyle())
                {
                case NShape::Rect:
                    ::Rectangle(alphaDc, rect.Left, rect.Top, rect.Right, rect.Bottom);
                    break;
                case NShape::Line:
                    ::MoveToEx(alphaDc, rect.Left, rect.Top, NULL);
                    ::LineTo(alphaDc, rect.Right, rect.Bottom);
                    break;
                case NShape::RoundRect:
                    ::RoundRect(alphaDc, rect.Left, rect.Top, rect.Right, rect.Bottom, 4, 4);
                    break;
                }
                alphaDc.EndDraw(brushAlpha);
            }
            else
            {
                if(brushAlpha > 0)
                {
                    CAlphaDC alphaDc;
                    alphaDc.Init(memDC_, rect, false);
                    Gdi::CGdiSelector brushSelector(alphaDc, brush, false);
                    Gdi::CGdiSelector penSelector(alphaDc, ::GetStockObject(NULL_PEN), false);

                    switch(shape->GetStyle())
                    {
                    case NShape::Rect:
                        ::Rectangle(alphaDc, rect.Left, rect.Top, rect.Right, rect.Bottom);
                        break;
                    case NShape::RoundRect:
                        ::RoundRect(alphaDc, rect.Left, rect.Top, rect.Right, rect.Bottom, 4, 4);
                        break;
                    }
                    alphaDc.EndDraw(brushAlpha);
                }
                if(penAlpha > 0)
                {
                    CAlphaDC alphaDc;
                    alphaDc.Init(memDC_, rect, false);

                    Gdi::CGdiSelector penSelector(alphaDc, pen, false);
                    Gdi::CGdiSelector brushSelector(alphaDc, ::GetStockObject(NULL_BRUSH), false);

                    switch(shape->GetStyle())
                    {
                    case NShape::Rect:
                        ::Rectangle(alphaDc, rect.Left, rect.Top, rect.Right, rect.Bottom);
                        break;
                    case NShape::Line:
                        ::MoveToEx(alphaDc, rect.Left, rect.Top, NULL);
                        ::LineTo(alphaDc, rect.Right, rect.Bottom);
                        break;
                    case NShape::RoundRect:
                        ::RoundRect(alphaDc, rect.Left, rect.Top, rect.Right, rect.Bottom, 4, 4);
                        break;
                    }
                    alphaDc.EndDraw(penAlpha);
                }
            }
        }

        void GdiRender::DrawImage(NImage* image, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue)
        {
            GdiImage* gdiImage = dynamic_cast<GdiImage*>(image);
            NAssertError(gdiImage != NULL, _T("Not GdiImage in GdiRender::DrawImage"));
            if(gdiImage == NULL)
                return;
            HBITMAP bitmap = gdiImage->GetHBitmap();
            NAssertError(bitmap != NULL, _T("GetHBitmap return NULL in GdiRender::DrawImage"));
            if(bitmap == NULL)
                return;
            ImageDC imageDc(memDC_, bitmap);

            BLENDFUNCTION BlendFunc = {0};
            BlendFunc.BlendOp = AC_SRC_OVER;
            BlendFunc.SourceConstantAlpha = alphaValue;
            BlendFunc.AlphaFormat = AC_SRC_ALPHA;
            BOOL bResult = ::AlphaBlend(memDC_,
                dstX, dstY,
                dstWidth, dstHeight,
                imageDc,
                srcX, srcY,
                srcWidth, srcHeight,
                BlendFunc);
            NAssertError(!!bResult, _T("AlphaBlend Failed in GdiRender::DrawImage"));
        }

        void GdiRender::DrawText(NText* text, const Base::NRect& rect)
        {
            GdiText* gdiText = dynamic_cast<GdiText*>(text);
            NAssertError(gdiText != NULL, _T("Not GdiText in GdiRender::DrawText"));
            if(gdiText == NULL)
                return;

            Base::NRect textRect(rect);
            Base::NRect tmpRect(rect);
            tmpRect.Offset(-tmpRect.Left, -tmpRect.Top);
            GetTextSize(text, tmpRect);

            if(gdiText->GetVertCenter())
            {
                textRect.Inflate(-(rect.Width() - tmpRect.Width()) / 2, -(rect.Height() - tmpRect.Height()) / 2);
            }
            else
            {
                textRect = tmpRect;
            }

            CAlphaDC alphaDc;
            alphaDc.Init(memDC_, textRect, true);

            Gdi::CGdiSelector fontSelector(alphaDc, ::GetCurrentObject(memDC_, OBJ_FONT), false);
            ::SetTextColor(alphaDc, gdiText->GetColor() & 0x00FFFFFF);
            ::DrawText(alphaDc, text->GetText(), text->GetText().GetLength(), textRect, gdiText->GetDrawFlags());

            alphaDc.EndDraw(GetAlpha(text->GetColor()));
        }

        void GdiRender::GetTextSize(NText *text, nui::Base::NRect &rect)
        {
            GdiText* gdiText = dynamic_cast<GdiText*>(text);
            NAssertError(gdiText != NULL, _T("Not GdiText in GdiRender::DrawText"));
            if(gdiText == NULL)
                return;

            DWORD flags = gdiText->GetDrawFlags();
            flags = (flags & (~DT_CENTER));
            flags = (flags & (~DT_VCENTER));
            flags = (flags & (~DT_END_ELLIPSIS));
            flags = (flags & (~DT_PATH_ELLIPSIS));
            flags = (flags & (~DT_WORD_ELLIPSIS));
            ::DrawText(memDC_, text->GetText(), text->GetText().GetLength(), rect, flags | DT_CALCRECT);
        }
    }
}
