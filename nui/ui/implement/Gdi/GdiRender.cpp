#include "stdafx.h"
#include "GdiRender.h"

#include "ImageDC.h"
#include "AlphaDC.h"
#include "GdiUtil.h"
#include "GdiShapeDraw.h"
#include "GdiImageDraw.h"
#include "GdiText.h"
#include "GdiFont.h"
#include "GdiUtil.h"

namespace nui
{
    namespace Ui
    {
        GdiRender::GdiRender()
        {
            orgDc_ = NULL;
        }

        GdiRender::~GdiRender()
        {}

        bool GdiRender::Init(HDC hDc, const Base::NRect& rcPaint)
        {
            orgDc_ = hDc;
            memDC_.Init(hDc, rcPaint, 255);
            return true;
        }

        void GdiRender::DrawBack(bool layered)
        {
            if(memDC_ == NULL)
                return;

            BLENDFUNCTION BlendFunc = {0};
            BlendFunc.BlendOp = AC_SRC_OVER;
            BlendFunc.SourceConstantAlpha = 255;
            BlendFunc.AlphaFormat = AC_SRC_ALPHA;

            const nui::Base::NPoint& pt = memDC_.GetLeftTop();
            const nui::Base::NSize& size = memDC_.GetSize();

            BOOL result;
            if(layered)
            {
                HWND window = ::WindowFromDC(orgDc_);
                Base::NRect  rcWnd;
                ::GetWindowRect(window, rcWnd);
                POINT ptTemp = {pt.X, pt.Y};
                SIZE sizeTemp = {rcWnd.Width(), rcWnd.Height()};
                result = ::UpdateLayeredWindow(window,
                    orgDc_,
                    rcWnd.GetLeftTop(),
                    &sizeTemp,
                    memDC_,
                    &ptTemp,
                    0,
                    &BlendFunc,
                    ULW_ALPHA);
            }
            else
            {
                result = ::AlphaBlend(orgDc_,
                    pt.X, pt.Y,
                    size.Width, size.Height,
                    memDC_,
                    0, 0,
                    size.Width, size.Height,
                    BlendFunc);
            }
            NAssertError(!!result, _T("AlphaBlend Failed"));
            memDC_.Destroy();
        }

        void GdiRender::DrawShape(NShape* shape, const Base::NRect& rect)
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
                if(alphaDc.Init(memDC_, rect, memDC_.GetSize(), false))
                {
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
            }
            else
            {
                if(brushAlpha > 0)
                {
                    CAlphaDC alphaDc;
                    if(alphaDc.Init(memDC_, rect, memDC_.GetSize(), false))
                    {
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
                }
                if(penAlpha > 0)
                {
                    CAlphaDC alphaDc;
                    if(alphaDc.Init(memDC_, rect, memDC_.GetSize(), false))
                    {
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
        }

        void GdiRender::DrawImage(NImage* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue)
        {
            GdiImage* gdiImage = dynamic_cast<GdiImage*>(image);
            NAssertError(gdiImage != NULL, _T("Not GdiImage in GdiRender::DrawImage"));
            if(gdiImage == NULL)
                return;
            HBITMAP bitmap = gdiImage->GetHBitmap(frameIndex);
            NAssertError(bitmap != NULL, _T("GetHBitmap return NULL in GdiRender::DrawImage"));
            if(bitmap == NULL)
                return;
            ImageDC imageDc(memDC_, bitmap);

            BLENDFUNCTION BlendFunc = {0};
            BlendFunc.BlendOp = AC_SRC_OVER;
            BlendFunc.SourceConstantAlpha = alphaValue;
            BlendFunc.AlphaFormat = AC_SRC_ALPHA;
            NVerify(!!::AlphaBlend(memDC_,
                dstX, dstY,
                dstWidth, dstHeight,
                imageDc,
                srcX, srcY,
                srcWidth, srcHeight,
                BlendFunc), _T("AlphaBlend Failed in GdiRender::DrawImage"));
        }

        void GdiRender::DrawText(NText* text, NFont* font, const Base::NRect& rect)
        {
            GdiText* gdiText = dynamic_cast<GdiText*>(text);
            GdiFont* gdiFont = NULL;
            NAssertError(gdiText != NULL, _T("Not GdiText in GdiRender::DrawText"));
            if(gdiText == NULL)
                return;

            if(font != NULL)
            {
                gdiFont = dynamic_cast<GdiFont*>(font);
                NAssertError(gdiFont != NULL, _T("Not GdiFont in GdiRender::DrawText"));
                if(gdiFont == NULL)
                    return;
            }

            Base::NRect textRect(rect);
            Base::NSize size(rect.Width(), rect.Height());
            GetTextSize(text, font, size);

            if(gdiText->GetVertCenter())
            {
                if(gdiText->GetHorzCenter())
                    textRect.Inflate(-(rect.Width() - size.Width) / 2, -(rect.Height() - size.Height) / 2);
                else
                    textRect.Inflate(0, -(rect.Height() - size.Height) / 2);
            }
            else
            {
                textRect.SetRect(0, 0, size.Width, size.Height);
                textRect.Offset(rect.Left, rect.Top);
            }

            // limit rect to be valid
            if(textRect.Top < rect.Top)
                textRect.Top = rect.Top;
            if(textRect.Left < rect.Left)
                textRect.Left = rect.Left;
            if(textRect.Right > rect.Right)
                textRect.Right = rect.Right;
            if(textRect.Bottom > rect.Bottom)
                textRect.Bottom = rect.Bottom;

            FillRectangle(textRect, gdiText->GetBgColor());

            CAlphaDC alphaDc;
            if(alphaDc.Init(memDC_, textRect, memDC_.GetSize(), true))
            {
                ::SetBkMode(alphaDc, TRANSPARENT);
                HGDIOBJ hFont = gdiFont == NULL ? ::GetCurrentObject(memDC_, OBJ_FONT) : gdiFont->GetFont();
                if(hFont == NULL)
                    hFont = ::GetCurrentObject(memDC_, OBJ_FONT);
                Gdi::CGdiSelector fontSelector(alphaDc, hFont, false);
                ::SetTextColor(alphaDc, gdiText->GetColor() & 0x00FFFFFF);
                ::DrawText(alphaDc, text->GetText(), text->GetText().GetLength(), textRect, gdiText->GetDrawFlags());

                alphaDc.EndDraw(GetAlpha(text->GetColor()));
            }
        }

        void GdiRender::GetTextSize(NText *text, NFont* font, nui::Base::NSize &size)
        {
            GdiText* gdiText = dynamic_cast<GdiText*>(text);
            NAssertError(gdiText != NULL, _T("Not GdiText in GdiRender::DrawText"));
            if(gdiText == NULL)
                return;

            HFONT hFont = NULL;
            GdiFont* gdiFont = NULL;
            if(font != NULL)
            {
                gdiFont = dynamic_cast<GdiFont*>(font);
                hFont = gdiFont->GetFont();
            }
            Gdi::CGdiSelector selector(memDC_, hFont, false);

            DWORD flags = gdiText->GetDrawFlags();
            flags = (flags & (~DT_CENTER));
            flags = (flags & (~DT_VCENTER));
            flags = (flags & (~DT_END_ELLIPSIS));
            flags = (flags & (~DT_PATH_ELLIPSIS));
            flags = (flags & (~DT_WORD_ELLIPSIS));
            Base::NRect rcTmp(0, 0, size.Width, size.Height);
            ::DrawText(memDC_, text->GetText(), text->GetText().GetLength(), rcTmp, flags | DT_CALCRECT);
            size.Width = rcTmp.Width();
            size.Height = rcTmp.Height();
        }

        nui::Base::NHolder GdiRender::ClipRect(const nui::Base::NRect& rect)
        {
            if(rect.Right <= rect.Left
                || rect.Bottom <= rect.Top)
            {
                Base::NHolder holder;
                return holder;
            }

            HRGN oldClip = ::CreateRectRgn(0, 0, 0, 0);
            ::GetClipRgn(memDC_, oldClip);

            HRGN hItemClip = ::CreateRectRgn(rect.Left, rect.Top, rect.Right, rect.Bottom);
            if(hItemClip != NULL)
            {
                ::ExtSelectClipRgn(memDC_, hItemClip, RGN_AND);
                ::DeleteObject(hItemClip);
            }

            Base::NHolder holder(reinterpret_cast<void*>(oldClip), MakeDelegate(this, &GdiRender::RestoreRgn));
            return holder;
        }

        MemDC& GdiRender::GetMemDC()
        {
            return memDC_;
        }

        void GdiRender::RestoreRgn(void* data)
        {
            if(data == NULL || memDC_ == NULL)
                return;

            HRGN oldClip = reinterpret_cast<HRGN>(data);

            ::ExtSelectClipRgn(memDC_, oldClip, RGN_COPY);
            ::DeleteObject(oldClip);
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
