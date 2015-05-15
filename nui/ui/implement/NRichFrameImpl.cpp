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
        }

        NRichFrame::~NRichFrame()
        {
            bkgDraw_ = NULL;
        }

        void NRichFrame::SetAutoSize(bool autosize)
        {
            if(autosize == IsAutoSize())
                return;
            Util::Misc::CheckFlag(frameFlags_, NRichFrame::FlagAutoSize, autosize);
            if(autosize)
                AutoSize();
        }

        bool NRichFrame::IsAutoSize() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NRichFrame::FlagAutoSize);
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
            Invalidate();
        }

        Base::NString NRichFrame::GetText() const
        {
            return text_ == NULL ? _T("") : text_->GetText();
        }

        NText* NRichFrame::GetRichText() const
        {
            return text_;
        }

        void NRichFrame::AutoSize()
        {
            NAssertError(window_ != NULL, _T("window is null"));
            if(!window_ || text_ == NULL)
                return;
            Base::NSize txtSize;
            window_->GetRender()->GetTextSize(text_, font_, txtSize);
            SetSize(txtSize.Width, txtSize.Height);
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
            return foreDraw_;
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
