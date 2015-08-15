#include "stdafx.h"
#include "../NEdit.h"


namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NEdit);

        NEdit::NEdit()
        {
            editWindow_ = NULL;
        }

        NEdit::~NEdit()
        {
            if(editWindow_)
            {
                ::DestroyWindow(editWindow_);
                editWindow_ = NULL;
            }
        }

        void NEdit::SetHintText(LPCTSTR szHintText)
        {
            NAssertError(IsEditValid(), _T("call parent->AddChild first"));
            if(!IsEditValid())
                return;

            nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
            Base::NString hintText = stringBundle->GetString(szHintText);
            ::SendMessage(editWindow_, EM_SETCUEBANNER, (WPARAM)hintText.GetData(), TRUE);
        }

        void NEdit::SetReadOnly(bool readOnly)
        {
            NAssertError(IsEditValid(), _T("call parent->AddChild first"));
            if(!IsEditValid())
                return;

            ::SendMessage(editWindow_, EM_SETREADONLY, readOnly, 0);
        }

        void NEdit::SelectAll()
        {
            NAssertError(IsEditValid(), _T("call parent->AddChild first"));
            if(!IsEditValid())
                return;

            ::SendMessage(editWindow_, EM_SETSEL, 0, -1);
        }

        void NEdit::ShowTooltip(TooltipIconType iconType, LPCTSTR szTitle, LPCTSTR szText)
        {
            NAssertError(IsEditValid(), _T("call parent->AddChild first"));
            if(!IsEditValid())
                return;

            nui::Base::NInstPtr<nui::Data::NStringBundle> stringBundle(MemToolParam);
            Base::NString strTitle = stringBundle->GetString(szTitle);
            Base::NString strText = stringBundle->GetString(szText);

            EDITBALLOONTIP tip = {sizeof(EDITBALLOONTIP)};
            tip.pszText = strText.GetData();
            tip.pszTitle = strTitle.GetData();
            tip.ttiIcon = iconType;
            ::SendMessage(editWindow_, EM_SHOWBALLOONTIP, 0, (LPARAM)&tip);
        }

        // Event
        void NEdit::SetTextChangeCallback(EditTextChangeEventCallback callback)
        {
            textChangeCallback_ = callback;
        }

        // NFrame
        // data
        void NEdit::SetText(const Base::NString& text)
        {
            __super::SetText(text);
            if(IsEditValid())
                ::SetWindowText(editWindow_, GetText().GetData());
        }

        Base::NString NEdit::GetText() const
        {
            if(IsEditValid())
            {
                TCHAR szText[1000];
                ::GetWindowText(editWindow_, szText, 999);
                szText[1000] = 0;
                return szText;
            }
            return __super::GetText();
        }

        NText* NEdit::GetRichText()
        {
            NText* pText = __super::GetRichText();
            if(pText == NULL)
                return NULL;
            pText->SetText(GetText().GetData());
            return pText;
        }

        bool NEdit::GetWndData(Base::NString& wndClassName, DWORD& style, DWORD& exStyle)
        {
            wndClassName = WC_EDIT;
            style = WS_TABSTOP | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_CHILD;
            exStyle = 0;
            return true;
        }

        bool NEdit::OnParentCommand(WORD notifyCode)
        {
            if(notifyCode != EN_CHANGE)
                return false;
            if(textChangeCallback_)
                textChangeCallback_(this);
            return true;
        }

        bool NEdit::IsEditValid() const
        {
            return editWindow_ != NULL && ::IsWindow(editWindow_);
        }

        // NFrameBase
        bool NEdit::SetPosImpl(int left, int top, bool force)
        {
            if(!__super::SetPosImpl(left, top, force) || !IsEditValid())
                return false;

            Base::NRect rcEdit = GetRootRect();
            ::SetWindowPos(editWindow_, NULL, rcEdit.Left, rcEdit.Top, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
            return true;
        }

        bool NEdit::SetSizeImpl(int width, int height, bool force)
        {
            if(!__super::SetSizeImpl(width, height, force) || !IsEditValid())
                return false;

            Base::NRect rcEdit = GetRootRect();
            ::SetWindowPos(editWindow_, NULL, 0, 0, rcEdit.Width(), rcEdit.Height(), SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
            return true;
        }
    }
}
