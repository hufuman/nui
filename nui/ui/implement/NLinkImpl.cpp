#include "stdafx.h"
#include "../NLink.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NLink);

        NLink::NLink()
        {
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagCanHover, true);
            GetTextAttr(NFrame::StatusNormal, true)->SetColor(MakeArgb(255, 0, 0, 204));
            GetFont();
            isUrl_ = false;
        }

        NLink::~NLink()
        {
        }

        NCursor* NLink::GetCursor() const
        {
            return GetCursorByType(NCursor::CursorHand);
        }

        void NLink::SetUrl(const Base::NString& url)
        {
            SetText(url);
            isUrl_ = !url.IsEmpty();
        }

        void NLink::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            NFont* font = GetFont();
            bool isHover = IsInStatus(NFrame::StatusHover);
            if(isHover)
                font->SetUnderline(true);
            __super::DrawContent(render, rect);
            if(isHover)
                font->SetUnderline(false);
        }

        void NLink::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:link"));
#endif  // _NO_NUI_PARSER_
        }

        void NLink::OnClicked(const nui::Base::NPoint& point)
        {
            __super::OnClicked(point);
            if(isUrl_)
                ::ShellExecute(window_->GetNative(), _T("open"), GetText(), NULL, NULL, SW_SHOW);
        }
    }
}
