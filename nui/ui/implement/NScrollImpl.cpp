#include "stdafx.h"
#include "../NScroll.h"

#include "../NRenderClip.h"
#include "../../util/NShellUtil.h"


#undef max
#undef min

namespace nui
{
    namespace Ui
    {
        const int g_minSliderWidth = 8;

        IMPLEMENT_REFLECTION(NScroll);

        NScroll::NScroll()
        {
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagCanHover, true);
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagAutoSize, true);

            scrollRange_ = 100;
            scrollPos_ = 0;

            scrollLine_ = 1;
            scrollPage_ = 10;
            horzScroll_ = false;

            hoverPart_ = ScrollPartNone;
            capturedPart_ = ScrollPartNone;

            RequireDraws();
            ReLayout();
        }

        NScroll::~NScroll()
        {
        }

        void NScroll::SetScrollRange(int range)
        {
            if(scrollRange_ == range)
                return;
            NAssertError(range > 0, _T("why you use scroll since range is zero?"));
            if(range < 1)
                return;
            scrollRange_ = range;
            if(scrollPos_ >= scrollRange_)
                SetScrollPos(scrollRange_ - 1);
            Invalidate();
        }

        void NScroll::SetScrollPos(int pos)
        {
            NAssertError(pos >= 0 && pos < scrollRange_, _T("wrong pos in NScroll?"));
            if(pos < 0)
                pos = 0;
            if(pos >= scrollRange_)
                pos = scrollRange_ - 1;
            if(scrollPos_ == pos)
                return;

            scrollPos_ = pos;

            ScrollEventData eventData;
            eventData.scrollPos = scrollPos_;
            ScrollEvent.Invoke(this, &eventData);
            Invalidate();
        }

        void NScroll::SetScrollLine(int line)
        {
            if(line > 0)
                scrollLine_ = line;
        }

        void NScroll::SetScrollPage(int page)
        {
            if(page < 1)
                page = 1;
            if(scrollPage_ == page)
                return;
            scrollPage_ = page;
            Invalidate();
        }

        void NScroll::SetScrollType(bool horzScroll)
        {
            if(horzScroll_ == horzScroll)
                return;
            horzScroll_ = horzScroll;

            blockDraw_ = NULL;
            sliderDraw_ = NULL;
            scrollBkgDraw_ = NULL;

            RequireDraws();
            AutoSize();
        }

        int NScroll::GetScrollPos() const
        {
            return scrollPos_;
        }

        int NScroll::GetScrollRange() const
        {
            return scrollRange_;
        }

        int NScroll::GetScrollLine() const
        {
            return scrollLine_;
        }

        int NScroll::GetScrollPage() const
        {
            return scrollPage_;
        }

        bool NScroll::SetVisible(bool visible)
        {
            bool result = __super::SetVisible(visible);

            if(result && visible)
            {
                ScrollEventData eventData;
                eventData.scrollPos = scrollPos_;
                ScrollEvent.Invoke(this, &eventData);
            }

            return result;
        }

        void NScroll::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:scroll"));
#endif  // _NO_NUI_PARSER_
        }

        Base::NSize NScroll::GetAutoSize() const
        {
            if(horzScroll_)
                return GetHorzAutoSize();
            return GetVertAutoSize();
        }

        Base::NSize NScroll::GetHorzAutoSize() const
        {
            Base::NSize autoSize;

            Base::NSize blockSize;
            Base::NSize bkgSize;
            Base::NSize sliderSize;

            if(blockDraw_)
                blockSize = blockDraw_->GetPreferSize();
            if(scrollBkgDraw_)
                bkgSize = scrollBkgDraw_->GetPreferSize();
            if(sliderDraw_)
                sliderSize = sliderDraw_->GetPreferSize();

            autoSize.Width += blockSize.Width * 2;
            autoSize.Width += std::max(bkgSize.Width, sliderSize.Width);
            autoSize.Height = std::max(std::max(blockSize.Height, bkgSize.Height), sliderSize.Height);
            return autoSize;
        }

        Base::NSize NScroll::GetVertAutoSize() const
        {
            Base::NSize autoSize;

            Base::NSize blockSize;
            Base::NSize bkgSize;
            Base::NSize sliderSize;

            if(blockDraw_)
                blockSize = blockDraw_->GetPreferSize();
            if(scrollBkgDraw_)
                bkgSize = scrollBkgDraw_->GetPreferSize();
            if(sliderDraw_)
                sliderSize = sliderDraw_->GetPreferSize();

            autoSize.Height += blockSize.Height * 2;
            autoSize.Height += std::max(bkgSize.Height, sliderSize.Height);
            autoSize.Width = std::max(std::max(blockSize.Width, bkgSize.Width), sliderSize.Width);
            return autoSize;
        }

        void NScroll::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            Base::NRect leftBlock;
            Base::NRect rightBlock;
            Base::NRect bkg;
            Base::NRect slider;

            if(horzScroll_)
                GetHorzPartRect(rect, leftBlock, rightBlock, bkg, slider);
            else
                GetVertPartRect(rect, leftBlock, rightBlock, bkg, slider);

            ScrollPart capturedPart = hoverPart_ == ScrollPartNone ? ScrollPartNone : capturedPart_;
            ScrollPart hoverPart = hoverPart_;

            if(blockDraw_)
            {
                // Left
                int index = capturedPart == ScrollPartLeftBlock ? 2 : (hoverPart == ScrollPartLeftBlock ? 1 : 0);
                blockDraw_->Draw(render, 0, index, leftBlock);

                // Right
                index = capturedPart == ScrollPartRightBlock ? 2 : (hoverPart == ScrollPartRightBlock ? 1 : 0);
                blockDraw_->Draw(render, 1, index, rightBlock);
            }

            if(scrollBkgDraw_)
            {
                int index = 0;
                if(capturedPart == ScrollPartLeftBkg || capturedPart == ScrollPartRightBkg)
                    index = 2;
                else if(hoverPart == ScrollPartLeftBkg || hoverPart == ScrollPartRightBkg)
                    index = 1;
                scrollBkgDraw_->Draw(render, 0, index, bkg);
            }

            if(sliderDraw_)
            {
                int index = capturedPart_ == ScrollPartSlider ? 2 : (hoverPart == ScrollPartSlider ? 1 : 0);
                sliderDraw_->Draw(render, 0, index, slider);
            }

            __super::DrawContent(render, rect);
        }

        void NScroll::OnMouseDown(short x, short y)
        {
            __super::OnMouseDown(x, y);
            RefreshCapturePart(x, y);

            if(capturedPart_ == ScrollPartNone)
                return;

            firstTimer_.Release();
            continousTimer_.Release();

            if(capturedPart_ == ScrollPartSlider)
            {
                Base::NRect itemRect = GetRootRect();
                Base::NRect leftBlock, rightBlock, bkg, slider;
                if(horzScroll_)
                    GetHorzPartRect(itemRect, leftBlock, rightBlock, bkg, slider);
                else
                    GetVertPartRect(itemRect, leftBlock, rightBlock, bkg, slider);

                startPoint_.SetPoint(x - slider.Left, y - slider.Top);
            }

            PosChangeTimerProc();

            Base::NInstPtr<Ui::NTimerSrv> timerSrv(MemToolParam);
            if(capturedPart_ == ScrollPartSlider)
                continousTimer_ = timerSrv->startTimer(50, MakeDelegate(this, &NScroll::PosChangeTimerProc));
            else
                firstTimer_ = timerSrv->startTimer(500, MakeDelegate(this, &NScroll::PosChangeTimerProc));
        }

        void NScroll::OnMouseUp()
        {
            __super::OnMouseUp();
            ResetCapturePart();
            firstTimer_.Release();
            continousTimer_.Release();
        }

        void NScroll::OnMouseLeave()
        {
            __super::OnMouseLeave();
            ResetHoverPart();
        }

        void NScroll::OnMouseMove(short x, short y)
        {
            __super::OnMouseMove(x, y);
            RefreshHoverPart(x, y);
        }

        void NScroll::SetBlockDraw(LPCTSTR filePath)
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            Base::NAutoPtr<NImageDraw> tmpDraw = loader->LoadImage(filePath);
            if(tmpDraw != NULL)
            {
                blockDraw_ = tmpDraw;
                AutoSize();
            }
        }

        void NScroll::SetSliderDraw(LPCTSTR filePath)
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            Base::NAutoPtr<NImageDraw> tmpDraw = loader->LoadImage(filePath);
            if(tmpDraw != NULL)
            {
                sliderDraw_ = tmpDraw;
                AutoSize();
            }
        }

        void NScroll::SetBkgDraw(LPCTSTR filePath)
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            Base::NAutoPtr<NImageDraw> tmpDraw = loader->LoadImage(filePath);
            if(tmpDraw != NULL)
            {
                scrollBkgDraw_ = tmpDraw;
                AutoSize();
            }
        }

        void NScroll::PosChangeTimerProc()
        {
            if(capturedPart_ == ScrollPartNone)
            {
                firstTimer_.Release();
                continousTimer_.Release();
                return;
            }

            Base::NPoint point = Util::Shell::GetCurrentPos(window_);

            ScrollPart part = FindPart(static_cast<short>(point.X), static_cast<short>(point.Y));

            int pos = GetScrollPos();
            if(capturedPart_ == ScrollPartLeftBlock && part == capturedPart_)
            {
                pos -= scrollLine_;
            }
            else if(capturedPart_ == ScrollPartLeftBkg && part == capturedPart_)
            {
                pos -= scrollPage_;
            }
            else if(capturedPart_ == ScrollPartSlider)
            {
                Base::NRect leftBlock;
                Base::NRect rightBlock;
                Base::NRect bkg;
                Base::NRect slider;

                if(horzScroll_)
                    GetHorzPartRect(GetRootRect(), leftBlock, rightBlock, bkg, slider);
                else
                    GetVertPartRect(GetRootRect(), leftBlock, rightBlock, bkg, slider);

                if(horzScroll_)
                {
                    if(bkg.Width() == slider.Width())
                    {
                        pos = 0;
                    }
                    else
                    {
                        double d = (double)(point.X - startPoint_.X - bkg.Left) * (scrollRange_ - 1) / (bkg.Width() - slider.Width());
                        pos = (int)(d + 0.5);
                    }
                }
                else
                {
                    if(bkg.Height() == slider.Height())
                    {
                        pos = 0;
                    }
                    else
                    {
                        double d = (double)(point.Y - startPoint_.Y - bkg.Top) * (scrollRange_ - 1) / (bkg.Height() - slider.Height());
                        pos = (int)(d + 0.5);
                    }
                }
            }
            else if(capturedPart_ == ScrollPartRightBkg && part == capturedPart_)
            {
                pos += scrollPage_;
            }
            else if(capturedPart_ == ScrollPartRightBlock && part == capturedPart_)
            {
                pos += scrollLine_;
            }

            if(pos < 0)
                pos = 0;
            if(pos >= scrollRange_)
                pos = scrollRange_ - 1;
            SetScrollPos(pos);

            if(firstTimer_)
            {
                firstTimer_.Release();
                Base::NInstPtr<Ui::NTimerSrv> timerSrv(MemToolParam);
                continousTimer_ = timerSrv->startTimer(50, MakeDelegate(this, &NScroll::PosChangeTimerProc));
            }
        }

        void NScroll::GetHorzPartRect(const Base::NRect& rect, Base::NRect& leftBlock, Base::NRect& rightBlock, Base::NRect& bkg, Base::NRect& slider) const
        {
            Base::NSize blockSize = blockDraw_->GetPreferSize();

            bkg = rect;
            if(blockDraw_)
            {
                // Left
                leftBlock.SetRect(rect.Left, rect.Top, rect.Left + blockSize.Width, rect.Bottom);

                // Right
                rightBlock.SetRect(rect.Right - blockSize.Width, rect.Top, rect.Right, rect.Bottom);

                // Bkg
                bkg.Left += blockSize.Width;
                bkg.Right -= blockSize.Width;
            }

            // Slider
            int foreWidth = 0;
            if(scrollPage_ < scrollRange_)
            {
                foreWidth = bkg.Width() * scrollPage_ / scrollRange_;
            }
            else
            {
                foreWidth = bkg.Width() * scrollPage_ / (scrollPage_ + scrollRange_);
            }
            foreWidth = std::max(g_minSliderWidth, foreWidth);
            int pos = (scrollRange_ > 1) ? scrollPos_ * (bkg.Width() - foreWidth) / (scrollRange_ - 1) : 0;
            slider.SetPos(bkg.Left + pos, rect.Top);
            slider.SetSize(foreWidth, rect.Height());
        }

        void NScroll::GetVertPartRect(const Base::NRect& rect, Base::NRect& topBlock, Base::NRect& bottomBlock, Base::NRect& bkg, Base::NRect& slider) const
        {
            bkg = rect;
            if(blockDraw_)
            {
                Base::NSize blockSize = blockDraw_->GetPreferSize();

                // Top
                topBlock.SetRect(rect.Left, rect.Top, rect.Right, rect.Top + blockSize.Height);

                // Bottom
                bottomBlock.SetRect(rect.Left, rect.Bottom - blockSize.Height, rect.Right, rect.Bottom);

                bkg.Top += blockSize.Height;
                bkg.Bottom -= blockSize.Height;
            }

            if(sliderDraw_)
            {
                int foreHeight = 0;
                if(scrollPage_ < scrollRange_)
                {
                    foreHeight = bkg.Height() * scrollPage_ / scrollRange_;
                }
                else
                {
                    foreHeight = bkg.Height() * scrollPage_ / (scrollPage_ + scrollRange_);
                }
                foreHeight = std::max(g_minSliderWidth, foreHeight);
                int pos = (scrollRange_ > 1) ? scrollPos_ * (bkg.Height() - foreHeight) / (scrollRange_ - 1) : 0;
                slider.SetPos(rect.Left, bkg.Top + pos);
                slider.SetSize(rect.Width(), foreHeight);
            }
        }

        void NScroll::RefreshCapturePart(short x, short y)
        {
            ScrollPart part = FindPart(x, y);
            if(part == capturedPart_)
                return;
            hoverPart_ = part;
            capturedPart_ = part;
            Invalidate();
        }

        void NScroll::ResetCapturePart()
        {
            if(capturedPart_ == ScrollPartNone)
                return;
            capturedPart_ = ScrollPartNone;
            Invalidate();
        }

        void NScroll::RefreshHoverPart(short x, short y)
        {
            // when capturedPart_ available, hoverPart_ can only be capturedPart_
            ScrollPart part = FindPart(x, y);
            if(capturedPart_ != ScrollPartNone && part != capturedPart_)
                part = ScrollPartNone;
            if(part == hoverPart_)
                return;
            hoverPart_ = part;
            Invalidate();
        }

        void NScroll::ResetHoverPart()
        {
            if(hoverPart_ == ScrollPartNone)
                return;
            hoverPart_ = ScrollPartNone;
            Invalidate();
        }

        NScroll::ScrollPart NScroll::FindPart(short x, short y)
        {
            Base::NRect itemRect = GetRootRect();
            Base::NRect leftBlock, rightBlock, bkg, slider;
            if(horzScroll_)
                GetHorzPartRect(itemRect, leftBlock, rightBlock, bkg, slider);
            else
                GetVertPartRect(itemRect, leftBlock, rightBlock, bkg, slider);

            if(leftBlock.Contains(x, y))
                return ScrollPartLeftBlock;
            if(rightBlock.Contains(x, y))
                return ScrollPartRightBlock;
            if(slider.Contains(x, y))
                return ScrollPartSlider;
            if(bkg.Contains(x, y))
            {
                if(horzScroll_)
                    return x < slider.Left ? ScrollPartLeftBkg : ScrollPartRightBkg;
                else
                    return y < slider.Top ? ScrollPartLeftBkg : ScrollPartRightBkg;
            }
            return ScrollPartNone;
        }

        void NScroll::RequireDraws()
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            blockDraw_ = loader->LoadImage(horzScroll_ ? _T("@skin:common\\horzScrollBlock.png") : _T("@skin:common\\vertScrollBlock.png"));
            sliderDraw_ = loader->LoadImage(horzScroll_ ? _T("@skin:common\\horzScrollSlider.png") : _T("@skin:common\\vertScrollSlider.png"));
            scrollBkgDraw_ = loader->LoadImage(horzScroll_ ? _T("@skin:common\\horzScrollBkg.png") : _T("@skin:common\\vertScrollBkg.png"));
            AutoSize();
        }
    }
}
