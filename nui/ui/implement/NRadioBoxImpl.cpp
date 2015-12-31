#include "stdafx.h"
#include "../NRadioBox.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NRadioBox);

        NRadioBox::NRadioBox()
        {
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagCanHover, true);
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            SetForeDraw(loader->LoadImage(_T("@skin:common\\radioBox.png")));
        }

        NRadioBox::~NRadioBox()
        {
        }

        void NRadioBox::SetGroup(const Base::NString& groupName)
        {
            groupName_ = groupName;
        }

        bool NRadioBox::SetCheck(bool checked)
        {
            if(parentFrame_ != NULL && checked)
                parentFrame_->EnumChilds(MakeDelegate(this, &NRadioBox::SyncRadioGroups), 0);
            return __super::SetCheck(checked);
        }

        void NRadioBox::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:radioBox"));
#endif  // _NO_NUI_PARSER_
        }

        void NRadioBox::OnClicked(const nui::Base::NPoint& point)
        {
            if(parentFrame_ != NULL)
                parentFrame_->EnumChilds(MakeDelegate(this, &NRadioBox::SyncRadioGroups), 0);
            __super::OnClicked(point);
            Invalidate();
        }

        bool NRadioBox::SyncRadioGroups(NFrameBase* frameBase, LPARAM)
        {
            NRadioBox* radioBox = dynamic_cast<NRadioBox*>(frameBase);
            if(!radioBox)
                return true;
            if(radioBox->groupName_ == groupName_)
                radioBox->SetCheck(false);
            return true;
        }
    }
}
