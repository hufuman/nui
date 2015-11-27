#include "stdafx.h"
#include "../NScroll.h"

#include "../NRenderClip.h"


#undef max
#undef min

namespace nui
{
    namespace Ui
    {
        // min width for slider
        const int g_minForeWidth = 8;

        IMPLEMENT_REFLECTION(NScroll);

        NScroll::NScroll()
        {
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagCanHover, true);
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagAutoSize, false);

            scrollRange_ = 100;
            scrollPos_ = 0;
            horzScroll_ = false;

            requireDraws();
        }

        NScroll::~NScroll()
        {
        }

        void NScroll::SetScrollRange(int range)
        {
            if(scrollRange_ == range)
                return;
            NAssertError(range > 0, _T("why you use scroll since range is zero?"));
            if(range < 0)
                return;
            if(scrollPos_ >= range)
                scrollPos_ = range - 1;
            scrollRange_ = range;
            Invalidate();
        }

        void NScroll::SetScrollPos(int pos)
        {
            NAssertError(pos >= 0 && pos < scrollRange_, _T("wrong pos in NScroll?"));
            if(pos < 0 || pos >= scrollRange_)
                return;
            if(scrollPos_ == pos)
                return;
            scrollPos_ = pos;
            Invalidate();
        }

        void NScroll::SetScrollType(bool horzScroll)
        {
            if(horzScroll_ == horzScroll)
                return;
            horzScroll_ = horzScroll;
            requireDraws();
            Invalidate();
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
            if(horzScroll_)
                DrawHorzContent(render, rect);
            else
                DrawVertContent(render, rect);
            __super::DrawContent(render, rect);
        }

        void NScroll::DrawHorzContent(NRender* render, const Base::NRect& rect) const
        {
            Base::NRect bkgRect(rect);
            if(blockDraw_)
            {
                Base::NRect blockRect;
                Base::NSize blockSize = blockDraw_->GetPreferSize();

                // Left
                blockRect.SetRect(rect.Left, rect.Top, rect.Left + blockSize.Width, rect.Bottom);
                blockDraw_->Draw(render, 0, 0, blockRect);

                // Right
                blockRect.SetRect(rect.Right - blockSize.Width, rect.Top, rect.Right, rect.Bottom);
                blockDraw_->Draw(render, 1, 0, blockRect);

                bkgRect.Left += blockSize.Width;
                bkgRect.Right -= blockSize.Width;
            }

            if(scrollBkgDraw_)
            {
                scrollBkgDraw_->Draw(render, 0, 0, bkgRect);
            }

            if(sliderDraw_)
            {
                Base::NRect drawRect;

                int foreWidth = bkgRect.Width() / scrollRange_;
                foreWidth = std::max(g_minForeWidth, foreWidth);
                int pos = scrollPos_ * (bkgRect.Width() - foreWidth) / (scrollRange_ - 1);
                drawRect.SetPos(bkgRect.Left + pos, rect.Top);
                drawRect.SetSize(foreWidth, rect.Height());
                sliderDraw_->Draw(render, 0, 0, drawRect);
            }
        }

        void NScroll::DrawVertContent(NRender* render, const Base::NRect& rect) const
        {
            Base::NRect bkgRect(rect);
            if(blockDraw_)
            {
                Base::NRect blockRect;
                Base::NSize blockSize = blockDraw_->GetPreferSize();

                // Top
                blockRect.SetRect(rect.Left, rect.Top, rect.Right, rect.Top + blockSize.Height);
                blockDraw_->Draw(render, 0, 0, blockRect);

                // Bottom
                blockRect.SetRect(rect.Left, rect.Bottom - blockSize.Height, rect.Right, rect.Bottom);
                blockDraw_->Draw(render, 1, 0, blockRect);

                bkgRect.Top += blockSize.Height;
                bkgRect.Bottom -= blockSize.Height;
            }

            if(scrollBkgDraw_)
            {
                scrollBkgDraw_->Draw(render, 0, 0, bkgRect);
            }

            if(sliderDraw_)
            {
                Base::NRect drawRect;

                int foreHeight = bkgRect.Height() / scrollRange_;
                foreHeight = std::max(g_minForeWidth, foreHeight);
                int pos = scrollPos_ * (bkgRect.Height() - foreHeight) / (scrollRange_ - 1);
                drawRect.SetPos(bkgRect.Left, bkgRect.Top + pos);
                drawRect.SetSize(rect.Width(), foreHeight);
                sliderDraw_->Draw(render, 0, 0, drawRect);
            }
        }

        void NScroll::requireDraws()
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            blockDraw_ = loader->LoadImage(horzScroll_ ? _T("@skin:common\\horzScrollBlock.png") : _T("@skin:common\\vertScrollBlock.png"));
            sliderDraw_ = loader->LoadImage(horzScroll_ ? _T("@skin:common\\horzScrollSlider.png") : _T("@skin:common\\vertScrollSlider.png"));
            scrollBkgDraw_ = loader->LoadImage(horzScroll_ ? _T("@skin:common\\horzScrollBkg.png") : _T("@skin:common\\vertScrollBkg.png"));
            AutoSize();
        }
    }
}
