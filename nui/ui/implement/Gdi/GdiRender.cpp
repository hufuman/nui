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

            BYTE penAlpha = gdiShape->GetBorderAlpha();
            BYTE brushAlpha = gdiShape->GetFillAlpha();

            if(penAlpha == 0 && brushAlpha == 0)
                return;

            int borderWidth = gdiShape->GetBorderWidth();
            ArgbColor borderColor = gdiShape->GetBorderColor();
            ArgbColor fillColor = gdiShape->GetFillColor();

            if(penAlpha == brushAlpha)
            {
                CAlphaDC alphaDc;
                alphaDc.Init(memDC_, rect, false);

                switch(shape->GetStyle())
                {
                case NShape::Rect:
                    {
                        DrawAndFillRectImpl(alphaDc, rect, borderWidth, borderColor, fillColor);
                    }
                    break;
                case NShape::Line:
                    {
                        Gdi::CGdiSelector selector(alphaDc, ::CreatePen(PS_SOLID, borderWidth, borderColor & 0x00FFFFFF), true);
                        ::MoveToEx(alphaDc, rect.Left, rect.Top, NULL);
                        ::LineTo(alphaDc, rect.Right, rect.Bottom);
                    }
                    break;
                case NShape::RoundRect:
                    {
                        Base::NRect rcRound(rect);
                        Gdi::CGdiHolder rgnHolder(::CreateRoundRectRgn(rcRound.Left, rcRound.Top, rcRound.Right + 1, rcRound.Bottom + 1, borderWidth * 4, borderWidth * 4), true);
                        Gdi::CGdiHolder fillBrushHolder(::CreateSolidBrush(fillColor & 0x00FFFFFF), true);
                        ::FillRgn(alphaDc, static_cast<HRGN>(static_cast<HGDIOBJ>(rgnHolder)), static_cast<HBRUSH>(static_cast<HGDIOBJ>(fillBrushHolder)));
                        Gdi::CGdiHolder borderBrushHolder(::CreateSolidBrush(borderColor & 0x00FFFFFF), true);
                        ::FrameRgn(alphaDc, static_cast<HRGN>(static_cast<HGDIOBJ>(rgnHolder)), static_cast<HBRUSH>(static_cast<HGDIOBJ>(borderBrushHolder)), borderWidth, borderWidth);
                    }
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

                    switch(shape->GetStyle())
                    {
                    case NShape::Rect:
                        {
                            Base::NRect rcTmp(rect);
                            rcTmp.Inflate(-borderWidth, -borderWidth);
                            FillRectImpl(alphaDc, rcTmp, fillColor);
                        }
                        break;
                    case NShape::RoundRect:
                        {
                            Base::NRect rcRound(rect);
                            Gdi::CGdiHolder brushHolder(::CreateSolidBrush(fillColor & 0x00FFFFFF), true);
                            Gdi::CGdiHolder rgnHolder(::CreateRoundRectRgn(rcRound.Left, rcRound.Top, rcRound.Right + 1, rcRound.Bottom + 1, borderWidth * 4, borderWidth * 4), true);
                            ::FillRgn(alphaDc, static_cast<HRGN>(static_cast<HGDIOBJ>(rgnHolder)), static_cast<HBRUSH>(static_cast<HGDIOBJ>(brushHolder)));
                        }
                        break;
                    }
                    alphaDc.EndDraw(brushAlpha);
                }
                if(penAlpha > 0)
                {
                    CAlphaDC alphaDc;
                    alphaDc.Init(memDC_, rect, false);

                    switch(shape->GetStyle())
                    {
                    case NShape::Rect:
                        {
                            DrawRectImpl(alphaDc, rect, borderWidth, borderColor);
                        }
                        break;
                    case NShape::Line:
                        {
                            Gdi::CGdiSelector selector(alphaDc, ::CreatePen(PS_SOLID, borderWidth, borderColor & 0x00FFFFFF), true);
                            ::MoveToEx(alphaDc, rect.Left, rect.Top, NULL);
                            ::LineTo(alphaDc, rect.Right, rect.Bottom);
                        }
                        break;
                    case NShape::RoundRect:
                        {
                            Base::NRect rcRound(rect);
                            Gdi::CGdiHolder brushHolder(::CreateSolidBrush(borderColor & 0x00FFFFFF), true);
                            Gdi::CGdiHolder rgnHolder(::CreateRoundRectRgn(rcRound.Left, rcRound.Top, rcRound.Right + 1, rcRound.Bottom + 1, borderWidth * 4, borderWidth * 4), true);
                            ::FrameRgn(alphaDc, static_cast<HRGN>(static_cast<HGDIOBJ>(rgnHolder)), static_cast<HBRUSH>(static_cast<HGDIOBJ>(brushHolder)), borderWidth, borderWidth);
                        }
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

        void GdiRender::FillRectImpl(HDC hDc, const Base::NRect& rect, ArgbColor fillColor)
        {
            DWORD color = ::SetBkColor(hDc, fillColor & 0x00FFFFFF);
            ::ExtTextOut(hDc, 0, 0, ETO_OPAQUE, (const RECT*)rect, NULL, 0, NULL);
            ::SetBkColor(hDc, color);
        }

        void GdiRender::DrawRectImpl(HDC hDc, const Base::NRect& rect, int width, ArgbColor borderColor)
        {
            Base::NRect rects[4];

            // left
            rects[0].SetRect(rect.Left, rect.Top, rect.Left + width, rect.Bottom);
            // top
            rects[1].SetRect(rect.Left, rect.Top, rect.Right, rect.Top + width);
            // right
            rects[2].SetRect(rect.Right - width, rect.Top, rect.Right, rect.Bottom);
            // bottom
            rects[3].SetRect(rect.Left, rect.Bottom - width, rect.Right, rect.Bottom);

            DWORD color = ::SetBkColor(hDc, borderColor & 0x00FFFFFF);
            for(int i=0; i<4; ++ i)
            {
                ::ExtTextOut(hDc, 0, 0, ETO_OPAQUE, (RECT*)&rects[i], NULL, 0, NULL);
            }
            ::SetBkColor(hDc, color);
        }

        void GdiRender::DrawAndFillRectImpl(HDC hDc, const Base::NRect& rect, int borderWidth, ArgbColor borderColor, ArgbColor fillColor)
        {
            Base::NRect rcFill(rect);
            rcFill.Inflate(-borderWidth, -borderWidth);
            FillRectImpl(hDc, rcFill, fillColor);
            DrawRectImpl(hDc, rect, borderWidth, borderColor);
        }
    }
}
