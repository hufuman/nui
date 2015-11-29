#include "stdafx.h"
#include "FrameParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NFrame.h"

IMPLEMENT_REFLECTION(FrameParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

FrameParserImpl::FrameParserImpl()
{
    text_ = NULL;
}

void FrameParserImpl::Create(nui::Base::NBaseObj* parentObj)
{
    NFrame* parentFrame = dynamic_cast<NFrame*>(parentObj);
    if(parentFrame == NULL)
        return;

    NFrame* frame = dynamic_cast<NFrame*>(targetObj_);
    NAssertError(frame != NULL, _T("Invalid obj in FrameParser"));

    NString frameId = frame->GetId();
    NRect rect = frame->GetRect();
    NString text = frame->GetText();

    frame->Create(parentFrame, frameId, rect, text);
}

void FrameParserImpl::PreParse(nui::Data::NDataReader* styleNode_, nui::Base::NBaseObj* target)
{
    FrameBaseParserImpl::PreParse(styleNode_, target);

    NFrame* targetFrame = dynamic_cast<NFrame*>(target);
    text_ = targetFrame->GetRichText();
}

void FrameParserImpl::PostParse()
{
    FrameBaseParserImpl::PostParse();
    text_ = NULL;
}

void FrameParserImpl::FillAttr()
{
    FrameBaseParserImpl::FillAttr();

    NFrame* targetFrame = dynamic_cast<NFrame*>(targetObj_);

    NString strTmp;
    ArgbColor color;
    bool bFlag;

    if(styleNode_->ReadValue(_T("text"), strTmp))
        targetFrame->SetText(strTmp);

    if(styleNode_->ReadValue(_T("textColor"), color))
        text_->SetColor(color);

    if(styleNode_->ReadValue(_T("textBgColor"), color))
        text_->SetBgColor(color);

    if(styleNode_->ReadValue(_T("textSingleLine"), bFlag))
        text_->SetSingleLine(bFlag);
}
