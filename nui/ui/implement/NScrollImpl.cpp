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
            Util::Misc::CheckFlag(frameFlags_, NRichFrame::FlagCanHover, true);
            Util::Misc::CheckFlag(frameFlags_, NRichFrame::FlagAutoSize, false);

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
            NAssertError(pos >= 0 && pos <= scrollRange_, _T("wrong pos in NScroll?"));
            if(pos < 0 || pos > scrollRange_)
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
            Base::NSize autoSize;

            NDraw* foreDraw = GetForeDraw();
            if(foreDraw != NULL)
                autoSize = foreDraw->GetPreferSize();
            NDraw* bkgDraw = GetBkgDraw();
            if(bkgDraw != NULL)
            {
                Base::NSize size = bkgDraw->GetPreferSize();
                autoSize.Width = std::max(autoSize.Width, size.Width);
                autoSize.Height = std::max(autoSize.Height, size.Height);
            }
            return autoSize;
        }

        void NScroll::DrawFore(NRender* render, const Base::NRect& rect) const
        {
            NAssertError(GetForeDraw() != NULL, _T("No Fore Draw"));

            Base::NRect bkgRect(rect);
            NDraw* bkgDraw = GetBkgDraw();
            NImageDraw* bkgImgDraw = dynamic_cast<NImageDraw*>(bkgDraw);
            if(bkgImgDraw != NULL)
            {
                const Base::NRect& rcParam = bkgImgDraw->GetStretchParam();
                if(horzScroll_)
                {
                    bkgRect.Left += rcParam.Left;
                    bkgRect.Right -= rcParam.Right;
                }
                else
                {
                    bkgRect.Top += rcParam.Top;
                    bkgRect.Bottom -= rcParam.Bottom;
                }
            }

            Base::NRect drawRect;

            if(horzScroll_)
            {
                int foreWidth = bkgRect.Width() / scrollRange_;
                foreWidth = std::max(g_minForeWidth, foreWidth);
                int pos = scrollPos_ * (bkgRect.Width() - foreWidth) / scrollRange_;
                drawRect.SetPos(bkgRect.Left + pos, rect.Top);
                drawRect.SetSize(foreWidth, rect.Height());
            }
            else
            {
                int foreHeight = bkgRect.Height() / scrollRange_;
                foreHeight = std::max(g_minForeWidth, foreHeight);
                int pos = scrollPos_ * (bkgRect.Height() - foreHeight) / scrollRange_;
                drawRect.SetPos(rect.Left, bkgRect.Top + pos);
                drawRect.SetSize(rect.Width(), foreHeight);
            }

            __super::DrawFore(render, drawRect);
        }

        void NScroll::requireDraws()
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            SetForeDraw(loader->LoadImage(horzScroll_ ? _T("@skin:common\\horzScrollFore.png") : _T("@skin:common\\vertScrollFore.png")));
            SetBkgDraw(loader->LoadImage(horzScroll_ ? _T("@skin:common\\horzScrollBkg.png") : _T("@skin:common\\vertScrollBkg.png")));
            AutoSize();
        }
    }
}
