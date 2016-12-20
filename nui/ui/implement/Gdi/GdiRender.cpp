#include "stdafx.h"
#include "GdiRender.h"

#include "ImageDC.h"
#include "AlphaDC.h"
#include "GdiUtil.h"
#include "GdiShapeDraw.h"
#include "GdiImageDraw.h"
#include "GdiTextAttr.h"
#include "GdiFont.h"
#include "GdiUtil.h"
#include "TempDC.h"
#include "GdiObjMgr.h"


namespace nui
{
    namespace Ui
    {
        HRGN g_EmptyRegion = ::CreateRectRgn(0, 0, 0, 0);

        GdiRender::GdiRender()
        {
            orgDc_ = NULL;
        }

        GdiRender::~GdiRender()
        {}

        bool GdiRender::Init(HDC hDc, const Base::NRect& rcPaint)
        {
            orgDc_ = hDc;
            memDC_.Init(orgDc_, rcPaint, 255);
            memDC_.SaveDc();
            ::SelectObject(memDC_, GdiObjMgr::Instance().GetDefaultFont());
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

        void GdiRender::DrawShape(NShapeDraw* shape, const Base::NRect& rect)
        {
            if(shape == NULL)
                return;
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::DrawShape"));
            GdiShapeDraw* gdiShapeDraw = dynamic_cast<GdiShapeDraw*>(shape);

            BYTE penAlpha = gdiShapeDraw->GetBorderAlpha();
            BYTE brushAlpha = gdiShapeDraw->GetFillAlpha();

            if(penAlpha == 0 && brushAlpha == 0)
                return;

            int borderWidth = gdiShapeDraw->GetBorderWidth();
            ArgbColor borderColor = gdiShapeDraw->GetBorderColor();
            ArgbColor fillColor = gdiShapeDraw->GetFillColor();

            if(penAlpha == brushAlpha)
            {
                CAlphaDC alphaDc;
                if(alphaDc.Init(memDC_, rect, memDC_.GetSize(), false))
                {
                    switch(shape->GetStyle())
                    {
                    case NShapeDraw::Rect:
                        {
                            DrawAndFillRectImpl(alphaDc, rect, borderWidth, borderColor, fillColor);
                        }
                        break;
                    case NShapeDraw::Line:
                        {
                            Gdi::CGdiSelector selector(alphaDc, ::CreatePen(PS_SOLID, borderWidth, borderColor & 0x00FFFFFF), true);
                            ::MoveToEx(alphaDc, rect.Left, rect.Top, NULL);
                            ::LineTo(alphaDc, rect.Right, rect.Bottom);
                        }
                        break;
                    case NShapeDraw::RoundRect:
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
                        case NShapeDraw::Rect:
                            {
                                Base::NRect rcTmp(rect);
                                rcTmp.Inflate(-borderWidth, -borderWidth);
                                FillRectImpl(alphaDc, rcTmp, fillColor);
                            }
                            break;
                        case NShapeDraw::RoundRect:
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
                        case NShapeDraw::Rect:
                            {
                                DrawRectImpl(alphaDc, rect, borderWidth, borderColor);
                            }
                            break;
                        case NShapeDraw::Line:
                            {
                                Gdi::CGdiSelector selector(alphaDc, ::CreatePen(PS_SOLID, borderWidth, borderColor & 0x00FFFFFF), true);
                                ::MoveToEx(alphaDc, rect.Left, rect.Top, NULL);
                                ::LineTo(alphaDc, rect.Right, rect.Bottom);
                            }
                            break;
                        case NShapeDraw::RoundRect:
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

        void GdiRender::DrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue)
        {
            switch(image->GetDrawType())
            {
            case ImageDrawType::Stretch:
                StretchDrawImage(image, frameIndex, srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, alphaValue);
                break;
            case ImageDrawType::Tile:
                TileDrawImage(image, frameIndex, srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, alphaValue);
                break;
            case ImageDrawType::NineStretch:
                NineStretchDrawImage(image, frameIndex, srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, alphaValue);
            case ImageDrawType::Cover:
                CoverDrawImage(image, frameIndex, srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight, alphaValue);
                break;
            default:
                NAssertError(false, _T("Wrong DrawImageType: %d"), image->GetDrawType());
            }
        }

        void GdiRender::DrawText(Base::NString text, NTextAttr* textAttr, NFont* font, const Base::NRect& rect)
        {
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::DrawText"));
            if(text.IsEmpty())
                return;

            GdiFont* gdiFont = NULL;
            GdiTextAttr* gdiTextAttr = NULL;

            if(font != NULL)
            {
                gdiFont = dynamic_cast<GdiFont*>(font);
                NAssertError(gdiFont != NULL, _T("Not GdiFont in GdiRender::DrawText"));
                if(gdiFont == NULL)
                    return;
            }

            if(textAttr != NULL)
            {
                gdiTextAttr = dynamic_cast<GdiTextAttr*>(textAttr);
                NAssertError(gdiTextAttr != NULL, _T("Not GdiTextAttr in GdiRender::DrawText"));
                if(gdiTextAttr == NULL)
                    return;
            }

            Base::NRect textRect(rect);
            Base::NSize size(rect.Width(), rect.Height());
            GetTextSize(text, textAttr, font, size, rect.Width());

            if(gdiTextAttr != NULL && gdiTextAttr->IsVertAlign())
            {
                if(gdiTextAttr->IsHorzAlign())
                    textRect.Inflate(-(rect.Width() - size.Width) / 2, -(rect.Height() - size.Height) / 2);
                else
                    textRect.Inflate(0, -(rect.Height() - size.Height) / 2);
            }
            else
            {
                textRect = rect;
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

            if(gdiTextAttr != NULL && GetAlpha(gdiTextAttr->GetBgColor()) > 0)
                FillRectangle(textRect, gdiTextAttr->GetBgColor());

            CAlphaDC alphaDc;
            if(alphaDc.Init(memDC_, textRect, memDC_.GetSize(), true))
            {
                ::SetBkMode(alphaDc, TRANSPARENT);
                HGDIOBJ hFont = gdiFont == NULL ? ::GetCurrentObject(memDC_, OBJ_FONT) : gdiFont->GetFont();
                if(hFont == NULL)
                    hFont = ::GetCurrentObject(memDC_, OBJ_FONT);
                Gdi::CGdiSelector fontSelector(alphaDc, hFont, false);

                if(gdiTextAttr != NULL)
                    ::SetTextColor(alphaDc, gdiTextAttr->GetColor() & 0x00FFFFFF);

                DWORD drawFlags = gdiTextAttr == NULL ? GDI_TEXTATTR_DEFAULT_DRAWFLAGS : gdiTextAttr->GetDrawFlags();
                ::DrawText(alphaDc, text, text.GetLength(), textRect, drawFlags | DT_EXPANDTABS);

                alphaDc.EndDraw(gdiTextAttr == NULL ? 255 : GetAlpha(gdiTextAttr->GetColor()));
            }
        }

        void GdiRender::GetTextSize(Base::NString text, NTextAttr *textAttr, NFont* font, nui::Base::NSize &size, int maxWidth)
        {
            NTempDC tmpDc;
            HDC hDc = memDC_;
            HGDIOBJ oldFont = NULL;
            if(hDc == NULL)
            {
                hDc = tmpDc.Init();
                oldFont = ::SelectObject(hDc, GdiObjMgr::Instance().GetDefaultFont());
            }

            HFONT hFont = NULL;
            GdiFont* gdiFont = NULL;
            if(font != NULL)
            {
                gdiFont = dynamic_cast<GdiFont*>(font);
                hFont = gdiFont->GetFont();
            }
            Gdi::CGdiSelector selector(hDc, hFont, false);

            DWORD drawFlags = GDI_TEXTATTR_DEFAULT_DRAWFLAGS;

            if(textAttr != NULL)
            {
                GdiTextAttr* gdiTextAttr = dynamic_cast<GdiTextAttr*>(textAttr);
                NAssertError(gdiTextAttr != NULL, _T("Not GdiTextAttr in GdiRender::GetTextSize"));
                if(gdiTextAttr == NULL)
                    return;
                drawFlags = gdiTextAttr->GetDrawFlags();
            }

            drawFlags = (drawFlags & (~DT_CENTER));
            drawFlags = (drawFlags & (~DT_VCENTER));
            drawFlags = (drawFlags & (~DT_BOTTOM));
            drawFlags = (drawFlags & (~DT_RIGHT));
            drawFlags = (drawFlags & (~DT_END_ELLIPSIS));
            drawFlags = (drawFlags & (~DT_PATH_ELLIPSIS));
            drawFlags = (drawFlags & (~DT_WORD_ELLIPSIS));
            Base::NRect rcTmp;
            if(size.Width == 0)
            {
                drawFlags = (drawFlags & (~DT_EDITCONTROL));
                rcTmp.SetRect(0, 0, 1000, 1000);
            }
            else
            {
                rcTmp.SetRect(0, 0, size.Width, 1000);
            }
#undef min
            if(maxWidth > 0)
                rcTmp.Right = std::min(rcTmp.Right, maxWidth);
            ::DrawText(hDc, text, text.GetLength(), rcTmp, drawFlags | DT_CALCRECT);
            size.Width = rcTmp.Width();
            size.Height = rcTmp.Height();

            if(oldFont != NULL)
                ::SelectObject(hDc, oldFont);
        }

        nui::Base::NHolder GdiRender::ClipRgn(HRGN clipRgn)
        {
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::ClipRect"));

            if(clipRgn == NULL || ::EqualRgn(clipRgn, g_EmptyRegion))
            {
                Base::NHolder holder;
                return holder;
            }

            Base::NRect clipRect;
            int result = ::GetClipBox(memDC_, clipRect);
            ::ExtSelectClipRgn(memDC_, clipRgn, RGN_AND);

            HRGN oldClip = NULL;
            if(result == SIMPLEREGION || result == COMPLEXREGION)
            {
                oldClip = ::CreateRectRgn(clipRect.Left, clipRect.Top, clipRect.Right, clipRect.Bottom);
            }

            Base::NHolder holder(reinterpret_cast<void*>(oldClip), MakeDelegate(this, &GdiRender::RestoreRgn));
            return holder;
        }

        MemDC& GdiRender::GetMemDC()
        {
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::GetMemDC"));

            return memDC_;
        }

        void GdiRender::RestoreRgn(void* data)
        {
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::RestoreRgn"));

            if(data == NULL || memDC_ == NULL)
                return;

            HRGN oldClip = reinterpret_cast<HRGN>(data);

            ::ExtSelectClipRgn(memDC_, oldClip, RGN_COPY);
            ::DeleteObject(oldClip);
        }

        void GdiRender::FillRectImpl(HDC hDc, const Base::NRect& rect, ArgbColor fillColor)
        {
            NAssertError(hDc != NULL, _T("hDc is null in GdiRender::FillRectImpl"));

            DWORD color = ::SetBkColor(hDc, fillColor & 0x00FFFFFF);
            ::ExtTextOut(hDc, 0, 0, ETO_OPAQUE, (const RECT*)rect, NULL, 0, NULL);
            ::SetBkColor(hDc, color);
        }

        void GdiRender::DrawRectImpl(HDC hDc, const Base::NRect& rect, int width, ArgbColor borderColor)
        {
            NAssertError(hDc != NULL, _T("hDc is null in GdiRender::DrawRectImpl"));

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
            NAssertError(hDc != NULL, _T("hDc is null in GdiRender::DrawAndFillRectImpl"));

            Base::NRect rcFill(rect);
            rcFill.Inflate(-borderWidth, -borderWidth);
            FillRectImpl(hDc, rcFill, fillColor);
            DrawRectImpl(hDc, rect, borderWidth, borderColor);
        }

        void GdiRender::StretchDrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue)
        {
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::StretchDrawImage"));

            if(srcX < 0 || srcY < 0
                || srcWidth <= 0 || srcHeight <= 0 || dstWidth <= 0 || dstHeight <= 0)
            {
                return;
            }

            GdiImageDraw* gdiImageDraw = dynamic_cast<GdiImageDraw*>(image);
            NAssertError(gdiImageDraw != NULL, _T("Not GdiImageDraw in GdiRender::StretchDrawImage"));
            if(gdiImageDraw == NULL)
                return;
            HBITMAP bitmap = gdiImageDraw->GetHBitmap(frameIndex);
            NAssertError(bitmap != NULL, _T("GetHBitmap return NULL in GdiRender::StretchDrawImage"));
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

        void GdiRender::TileDrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue)
        {
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::TileDrawImage"));

            if(srcX < 0 || srcY < 0 || dstX < 0 || dstY < 0
                || srcWidth <= 0 || srcHeight <= 0 || dstWidth <= 0 || dstHeight <= 0)
            {
                return;
            }

            int horzFullCount = dstWidth / srcWidth;
            int vertFullCount = dstHeight / srcHeight;

            // draw full image first
            for(int x=0; x<horzFullCount; ++ x)
            {
                for(int y=0; y<vertFullCount; ++ y)
                {
                    StretchDrawImage(image, frameIndex, srcX, srcY, srcWidth, srcHeight, dstX + x * srcWidth, dstY + y * srcHeight, srcWidth, srcHeight, alphaValue);
                }
            }

            // draw right most half image if need
            int rightHalfWidth = dstWidth % srcWidth;
            if(rightHalfWidth > 0)
            {
                for(int y=0; y<vertFullCount; ++ y)
                {
                    StretchDrawImage(image, frameIndex, srcX, srcY, rightHalfWidth, srcHeight, dstX + horzFullCount * srcWidth, dstY + y * srcHeight, rightHalfWidth, srcHeight, alphaValue);
                }
            }

            // draw bottom half image if need
            int bottomHalfHeight = dstHeight % srcHeight;
            if(bottomHalfHeight > 0)
            {
                for(int x=0; x<horzFullCount; ++ x)
                {
                    StretchDrawImage(image, frameIndex, srcX, srcY, srcWidth, bottomHalfHeight, dstX + x * srcWidth, dstY + vertFullCount * srcHeight, srcWidth, bottomHalfHeight, alphaValue);
                }
            }

            // draw right-bottom corner
            if(rightHalfWidth > 0 && bottomHalfHeight > 0)
            {
                StretchDrawImage(image, frameIndex, srcX, srcY, rightHalfWidth, bottomHalfHeight, dstX + horzFullCount * srcWidth, dstY + vertFullCount * srcHeight, rightHalfWidth, bottomHalfHeight, alphaValue);
            }
        }

        void GdiRender::NineStretchDrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue)
        {
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::NineStretchDrawImage"));

            if(srcX < 0 || srcY < 0
                || srcWidth <= 0 || srcHeight <= 0 || dstWidth <= 0 || dstHeight <= 0)
            {
                return;
            }

            Base::NRect rcParam = image->GetStretchParam();

            int srcDrawWidth, srcDrawHeight, dstDrawWidth, dstDrawHeight;
#undef min
            // LeftTop
            srcDrawWidth = std::min(rcParam.Left, srcWidth);
            srcDrawHeight = std::min(rcParam.Top, srcHeight);
            dstDrawWidth = std::min(rcParam.Left, dstWidth);
            dstDrawHeight = std::min(rcParam.Top, dstHeight);

            StretchDrawImage(image, frameIndex,
                srcX, srcY, srcDrawWidth, srcDrawHeight,
                dstX, dstY, dstDrawWidth, dstDrawHeight,
                alphaValue);
            // Left
            StretchDrawImage(image, frameIndex,
                srcX, srcY + srcDrawHeight, srcDrawWidth, srcHeight - rcParam.Top - rcParam.Bottom,
                dstX, dstY + dstDrawHeight, dstDrawWidth, dstHeight - rcParam.Top - rcParam.Bottom,
                alphaValue);
            // LeftBottom
            if(srcHeight > rcParam.Bottom && dstHeight > rcParam.Bottom)
            {
                srcDrawHeight = std::min(rcParam.Bottom, srcHeight);
                dstDrawHeight = std::min(rcParam.Bottom, dstHeight);
                StretchDrawImage(image, frameIndex,
                    srcX, srcY + srcHeight - rcParam.Bottom, srcDrawWidth, srcDrawHeight,
                    dstX, dstY + dstHeight - rcParam.Bottom, dstDrawWidth, dstDrawHeight,
                    alphaValue);
            }

            // Top
            srcDrawHeight = std::min(rcParam.Top, srcHeight);
            dstDrawHeight = std::min(rcParam.Top, dstHeight);
            StretchDrawImage(image, frameIndex,
                srcX + rcParam.Left, srcY, srcWidth - rcParam.Left - rcParam.Right, srcDrawHeight,
                dstX + rcParam.Left, dstY, dstWidth - rcParam.Left - rcParam.Right, dstDrawHeight,
                alphaValue);
            // Bottom
            if(srcHeight > rcParam.Bottom && dstHeight > rcParam.Bottom)
            {
                srcDrawHeight = std::min(rcParam.Bottom, srcHeight);
                dstDrawHeight = std::min(rcParam.Bottom, dstHeight);
                StretchDrawImage(image, frameIndex,
                    srcX + rcParam.Left, srcY + srcHeight - rcParam.Bottom, srcWidth - rcParam.Left - rcParam.Right, rcParam.Bottom,
                    dstX + rcParam.Left, dstY + dstHeight - rcParam.Bottom, dstWidth - rcParam.Left - rcParam.Right, rcParam.Bottom,
                    alphaValue);
            }

            srcDrawWidth = std::min(rcParam.Right, srcWidth);
            srcDrawHeight = std::min(rcParam.Top, srcHeight);
            dstDrawWidth = std::min(rcParam.Right, dstWidth);
            dstDrawHeight = std::min(rcParam.Top, dstHeight);
            if(srcWidth > rcParam.Right && dstWidth > rcParam.Right)
            {
                // RightTop
                StretchDrawImage(image, frameIndex,
                    srcX + srcWidth - rcParam.Right, srcY, srcDrawWidth, srcDrawHeight,
                    dstX + dstWidth - rcParam.Right, dstY, dstDrawWidth, dstDrawHeight,
                    alphaValue);
                // Right
                StretchDrawImage(image, frameIndex,
                    srcX + srcWidth - rcParam.Right, srcY + srcDrawHeight, rcParam.Right, srcHeight - rcParam.Top - rcParam.Bottom,
                    dstX + dstWidth - rcParam.Right, dstY + dstDrawHeight, rcParam.Right, dstHeight - rcParam.Top - rcParam.Bottom,
                    alphaValue);
            }

            if(srcWidth > rcParam.Right && srcHeight > rcParam.Bottom
                && dstWidth > rcParam.Right && dstHeight > rcParam.Bottom)
            {
                // RightBottom
                StretchDrawImage(image, frameIndex,
                    srcX + srcWidth - rcParam.Right, srcY + srcHeight - rcParam.Bottom, rcParam.Right, rcParam.Bottom,
                    dstX + dstWidth - rcParam.Right, dstY + dstHeight - rcParam.Bottom, rcParam.Right, rcParam.Bottom,
                    alphaValue);
            }

            // Center
            srcDrawWidth = std::min(rcParam.Left, srcWidth);
            srcDrawHeight = std::min(rcParam.Top, srcHeight);
            dstDrawWidth = std::min(rcParam.Left, dstWidth);
            dstDrawHeight = std::min(rcParam.Top, dstHeight);
            StretchDrawImage(image, frameIndex,
                srcX + srcDrawWidth, srcY + srcDrawHeight, srcWidth - rcParam.Left - rcParam.Right, srcHeight - rcParam.Top - rcParam.Bottom,
                dstX + dstDrawWidth, dstY + dstDrawHeight, dstWidth - rcParam.Left - rcParam.Right, dstHeight - rcParam.Top - rcParam.Bottom,
                alphaValue);
        }

        void GdiRender::CoverDrawImage(NImageDraw* image, int frameIndex, int srcX, int srcY, int srcWidth, int srcHeight, int dstX, int dstY, int dstWidth, int dstHeight, BYTE alphaValue)
        {
            NAssertError(memDC_ != NULL, _T("memDC_ is null in GdiRender::CoverDrawImage"));

            if(srcX < 0 || srcY < 0
                || srcWidth <= 0 || srcHeight <= 0
                || dstWidth <= 0 || dstHeight <= 0)
            {
                return;
            }

            int width = 0;
            int height = 0;
            if((double)srcWidth / srcHeight > (double)dstWidth / dstHeight)
            {
                width = static_cast<int>(static_cast<double>(srcHeight * dstWidth / dstHeight));
                height = srcHeight;
            }
            else
            {
                width = srcWidth;
                height = static_cast<int>(static_cast<double>(srcWidth) * dstHeight / dstWidth);
            }

            // Center
            StretchDrawImage(image, frameIndex,
                srcX, srcY, width, height,
                dstX, dstY, dstWidth, dstHeight,
                alphaValue);
        }
    }
}
