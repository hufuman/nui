#include "stdafx.h"
#include "../NEdit.h"


namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NEdit);

        NEdit::NEdit()
        {
        }

        NEdit::~NEdit()
        {
        }

        void NEdit::SetHintText(LPCTSTR szHintText)
        {
            NAssertError(IsWndValid(), _T("call parent->AddChild first"));
            if(!IsWndValid())
                return;

            nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
            Base::NString hintText = stringBundle->GetString(szHintText);
            ::SendMessage(realWindow_, EM_SETCUEBANNER, TRUE, (LPARAM)hintText.GetData());
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
            return true;
        }

        void NEdit::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:edit"));
#endif  // _NO_NUI_PARSER_
        }

        bool NEdit::OnParentCommand(WORD notifyCode)
        {
            if(notifyCode != EN_CHANGE)
                return false;
            TextChangeEventData eventData;
            TextChangeEvent.Invoke(this, &eventData);
            return true;
        }
    }
}
