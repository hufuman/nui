#include "stdafx.h"
#include "GdiRender.h"

#include "ImageDC.h"
#include "AlphaDC.h"
#include "GdiUtil.h"
#include "GdiShape.h"
#include "GdiImage.h"

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
    }
}
