#include "stdafx.h"
#include "../NCheckBox.h"

#include "../NRenderClip.h"

#undef max

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NCheckBox);

        const int CheckTextAndImageMargin = 8;

        Base::NRect GetCenterRect(const Base::NRect &rect, const Base::NSize &size)
        {
            Base::NRect result;
            if(rect.Height() >= size.Height)
            {
                result.SetPos(rect.Left, rect.Top + (rect.Height() - size.Height) / 2);
                result.SetSize(size.Width, size.Height);
            }
            else
            {
                result.SetPos(rect.Left, rect.Top);
                result.SetSize(size.Width * rect.Height() / size.Height, size.Height);
            }
            return result;
        }

        NCheckBox::NCheckBox()
        {
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagCanHover, true);
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            SetBkgDraw(NULL);
            SetForeDraw(loader->LoadImage(_T("@skin:common\\checkBox.png")));

            scaleImage_ = false;
        }

        NCheckBox::~NCheckBox()
        {
        }

        void NCheckBox::SetScaleImage(bool scaleImage)
        {
            scaleImage_ = scaleImage;
            Invalidate();
        }

        Base::NSize NCheckBox::GetAutoSize() const
        {
            if(scaleImage_ || window_ == NULL || window_->GetRender() == NULL)
                return __super::GetAutoSize();

            Base::NSize autoSize;

            Base::NString text = GetText();
            if(!text.IsEmpty())
            {
                window_->GetRender()->GetTextSize(text, GetTextAttr(), GetFont(), autoSize);
            }

            if(GetForeDraw() != NULL)
            {
                Base::NSize foreSize = GetForeDraw()->GetPreferSize();

                if(!text.IsEmpty())
                    autoSize.Width += CheckTextAndImageMargin;
                autoSize.Width += foreSize.Width;
                autoSize.Height = std::max(autoSize.Height, foreSize.Height);
            }

            if(GetBkgDraw() != NULL)
            {
                Base::NSize foreSize = GetBkgDraw()->GetPreferSize();
                autoSize.Width = std::max(autoSize.Width, foreSize.Width);
                autoSize.Height = std::max(autoSize.Height, foreSize.Height);
            }
            return autoSize;
        }

        void NCheckBox::DrawFore(NRender* render, const Base::NRect& rect) const
        {
            if(!GetForeDraw())
                return;

            if(scaleImage_)
            {
                __super::DrawFore(render, rect);

                // draw text, or text will be covered
                if(!GetText().IsEmpty())
                    render->DrawText(GetText(), GetTextAttr(), GetFont(), rect);
                return;
            }

            Base::NRect rcFore = GetCenterRect(rect, GetForeDraw()->GetPreferSize());
            int horzIndex, vertIndex;
            GetDrawIndex(horzIndex, vertIndex);
            GetForeDraw()->Draw(render, horzIndex, vertIndex, rcFore);
        }

        void NCheckBox::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            if(scaleImage_)
            {
                __super::DrawContent(render, rect);
                return;
            }

            Base::NString text = GetText();
            if(text.IsEmpty())
                return;

            Base::NRect rcFore = GetCenterRect(rect, GetForeDraw()->GetPreferSize());
            Base::NRect rcText(rcFore);
            rcText.Left = rcText.Right + CheckTextAndImageMargin;
            rcText.Right = rect.Right;
            render->DrawText(text, GetTextAttr(), GetFont(), rect);
        }

        void NCheckBox::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:checkBox"));
#endif  // _NO_NUI_PARSER_
        }

        void NCheckBox::OnClicked(const nui::Base::NPoint& point)
        {
            __super::OnClicked(point);

            Util::Misc::CheckFlag(frameStatus_, NFrame::StatusChecked, !IsInStatus(NFrame::StatusChecked));
            Invalidate();
        }
    }
}
