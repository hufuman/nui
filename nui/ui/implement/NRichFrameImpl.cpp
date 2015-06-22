#include "stdafx.h"
#include "../NRichFrame.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NRichFrame);

        NRichFrame::NRichFrame()
        {
            bkgDraw_ = NULL;
            Util::Misc::CheckFlag(frameFlags_, NRichFrame::FlagAutoSize, true);
        }

        NRichFrame::~NRichFrame()
        {
            bkgDraw_ = NULL;
            foreDraw_ = NULL;
        }

        void NRichFrame::OnParentChanged()
        {
            __super::OnParentChanged();
            if(GetParent() != NULL)
                AutoSize();
        }

        void NRichFrame::SetText(const Base::NString& text)
        {
            if(text.IsEmpty() && text_  == NULL)
                return;
            if(text_ == NULL)
            {
                text_ = NUiBus::Instance().GetResourceLoader()->CreateText(text, MemToolParam);
            }
            else
            {
                if(text_->GetText() == text)
                    return;
                text_->SetText(text);
            }
            AutoSize();
            Invalidate();
        }

        const Base::NString& NRichFrame::GetText() const
        {
            return text_ == NULL ? Base::NString::EmptyString : text_->GetText();
        }

        NText* NRichFrame::GetRichText() const
        {
            return text_;
        }

        Base::NSize NRichFrame::GetAutoSize() const
        {
            if(text_ == NULL && foreDraw_ == NULL && bkgDraw_ == NULL)
            {
                Base::NSize autoSize(frameRect_.Width(), frameRect_.Height());
                return autoSize;
            }

#undef max

            Base::NSize autoSize;
            if(text_ != NULL && window_ != NULL)
            {
                window_->GetRender()->GetTextSize(text_, font_, autoSize);
            }

            if(foreDraw_ != NULL)
            {
                Base::NSize foreSize = foreDraw_->GetPreferSize();
                autoSize.Width = std::max(autoSize.Width, foreSize.Width);
                autoSize.Height = std::max(autoSize.Height, foreSize.Height);
            }

            if(bkgDraw_ != NULL)
            {
                Base::NSize bkgSize = bkgDraw_->GetPreferSize();
                autoSize.Width = std::max(autoSize.Width, bkgSize.Width);
                autoSize.Height = std::max(autoSize.Height, bkgSize.Height);
            }
            return autoSize;
        }

        void NRichFrame::SetBkgDraw(NDraw* bkgDraw)
        {
            if(bkgDraw == bkgDraw_)
                return;
            bkgDraw_ = bkgDraw;
            Invalidate();
        }

        NDraw* NRichFrame::GetBkgDraw() const
        {
            return bkgDraw_;
        }

        void NRichFrame::SetForeDraw(NDraw* foreDraw)
        {
            if(foreDraw == foreDraw_)
                return;
            foreDraw_ = foreDraw;
            Invalidate();
        }

        NDraw* NRichFrame::GetForeDraw() const
        {
            return foreDraw_;
        }

        // Draw
        void NRichFrame::DrawBkg(NRender* render, const Base::NRect& rect) const
        {
            if(bkgDraw_ == NULL)
                return;
            int horzIndex, vertIndex;
            GetDrawIndex(horzIndex, vertIndex);
            bkgDraw_->Draw(render, horzIndex, vertIndex, rect);
        }

        void NRichFrame::DrawFore(NRender* render, const Base::NRect& rect) const
        {
            if(foreDraw_ == NULL)
                return;
            int horzIndex, vertIndex;
            GetDrawIndex(horzIndex, vertIndex);
            foreDraw_->Draw(render, horzIndex, vertIndex, rect);
        }

        void NRichFrame::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            if(text_ != NULL)
                render->DrawText(text_, font_, rect);
        }
    }
}
