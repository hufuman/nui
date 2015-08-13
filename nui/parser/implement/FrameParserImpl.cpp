#include "stdafx.h"
#include "FrameParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NFrame.h"

IMPLEMENT_REFLECTION(FrameParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;


void FrameParserImpl::PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target)
{
    FrameBaseParserImpl::PreParse(styleNode, target);

    NFrame* targetFrame = dynamic_cast<NFrame*>(targetObj_);
    text_ = targetFrame->GetRichText();
}

void FrameParserImpl::PostParse()
{
    FrameBaseParserImpl::PostParse();
    text_ = NULL;
}

bool FrameParserImpl::SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue)
{
    if(FrameBaseParserImpl::SetAttr(attrName, attrValue))
        return true;

    NFrame* targetFrame = dynamic_cast<NFrame*>(targetObj_);

    if(attrName == _T("text"))
    {
        targetFrame->SetText(attrValue);
    }
    else if(attrName == _T("textColor"))
    {
        text_->SetColor(ParserUtil::ParseArgb(attrValue));
    }
    else if(attrName == _T("textBgColor"))
    {
        text_->SetBgColor(ParserUtil::ParseArgb(attrValue));
    }
    else if(attrName == _T("textSingleLine"))
    {
        text_->SetSingleLine(ParserUtil::ParseBool(attrValue));
    }
    else if(attrName == _T("textVertCenter"))
    {
        text_->SetVertCenter(ParserUtil::ParseBool(attrValue));
    }
    else if(attrName == _T("textHorzCenter"))
    {
        text_->SetHorzCenter(ParserUtil::ParseBool(attrValue));
    }
    else
    {
        return false;
    }

    return true;
}
