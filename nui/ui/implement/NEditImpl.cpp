#include "stdafx.h"
#include "../NEdit.h"

#include "./Gdi/GdiFont.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NEdit);

        NEdit::NEdit()
        {
            GetTextAttr(NFrame::StatusNormal, true)->SetSingleLine(true)->SetAlignFlags(NTextAttr::TextAlignLeft | NTextAttr::TextAlignTop);
        }

        NEdit::~NEdit()
        {
        }

        void NEdit::SetHintText(LPCTSTR szHintText)
        {
            nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
            hintText_ = stringBundle->GetString(szHintText);
            if(!IsWndValid())
                return;

            ::SendMessage(realWindow_, EM_SETCUEBANNER, TRUE, (LPARAM)hintText_.GetData());
        }

        void NEdit::SetReadOnly(bool readOnly)
        {
            NAssertError(IsWndValid(), _T("call parent->AddChild first"));
            if(!IsWndValid())
                return;

            ::SendMessage(realWindow_, EM_SETREADONLY, readOnly, 0);
        }

        void NEdit::SelectAll()
        {
            NAssertError(IsWndValid(), _T("call parent->AddChild first"));
            if(!IsWndValid())
                return;

            ::SendMessage(realWindow_, EM_SETSEL, 0, -1);
        }

        void NEdit::ShowTooltip(TooltipIconType iconType, LPCTSTR szTitle, LPCTSTR szText)
        {
            NAssertError(IsWndValid(), _T("call parent->AddChild first"));
            if(!IsWndValid())
                return;

            nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
            Base::NString strTitle = stringBundle->GetString(szTitle);
            Base::NString strText = stringBundle->GetString(szText);

            EDITBALLOONTIP tip = {sizeof(EDITBALLOONTIP)};
            tip.pszText = strText.GetData();
            tip.pszTitle = strTitle.GetData();
            tip.ttiIcon = iconType;
            ::SendMessage(realWindow_, EM_SHOWBALLOONTIP, 0, (LPARAM)&tip);
        }

        bool NEdit::GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle)
        {
            wndClassName = WC_EDIT;
            style = WS_TABSTOP | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD;
            exStyle = 0;

            NTextAttr* textAttr = GetTextAttr();
            if(textAttr && !textAttr->IsSingleLine())
            {
                style |= ES_WANTRETURN | WS_HSCROLL | WS_VSCROLL | ES_MULTILINE;
            }

            return true;
        }

        void NEdit::OnRealWindowCreated()
        {
            __super::OnRealWindowCreated();

            NTextAttr* textAttr = GetTextAttr();
            if(textAttr && textAttr->IsSingleLine())
            {
                Base::NSize textSize;
                if(!GetText().IsEmpty() && window_->GetRender())
                {
                    window_->GetRender()->GetTextSize(GetText(), GetTextAttr(), GetFont(), textSize, maxSize_.Width);

                    UINT flags = textAttr->GetAlignFlags();
                    if(flags != 0)
                    {
                        int left = 0;
                        int top = 0;

                        if(flags & NTextAttr::TextAlignRight)
                            left = frameRect_.Width() - textSize.Width;
                        else if(flags & NTextAttr::TextAlignHCenter)
                            left = (frameRect_.Width() - textSize.Width) / 2;

                        if(flags & NTextAttr::TextAlignBottom)
                            top = frameRect_.Height() - textSize.Height;
                        else if(flags & NTextAttr::TextAlignHCenter)
                            top = (frameRect_.Height() - textSize.Height) / 2;

                        Base::NRect rcText;
                        rcText.SetPos(left, top);
                        rcText.SetSize(textSize.Width, textSize.Height);
                        ::SendMessage(realWindow_, EM_SETRECT, 0, reinterpret_cast<LPARAM>((RECT*)rcText));
                    }
                }
            }
            ::SendMessage(realWindow_, EM_SETCUEBANNER, TRUE, (LPARAM)hintText_.GetData());
            // fixme: multiple line edit should use nui's scrollbar
            ::SetFocus(realWindow_);
            Edit_SetSel(realWindow_, 0, -1);
        }

        void NEdit::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:edit"));
#endif  // _NO_NUI_PARSER_
        }

        NCursor* NEdit::GetCursor() const
        {
            return GetCursorByType(NCursor::CursorBeam);
        }

        bool NEdit::OnParentCommand(WORD notifyCode)
        {
            if(notifyCode == EN_CHANGE)
            {
                TextChangedEvent.Invoke(this, NULL);
            }
            return true;
        }
    }
}
