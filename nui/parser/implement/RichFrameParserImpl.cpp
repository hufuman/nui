#include "stdafx.h"
#include "RichFrameParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NRichFrame.h"

IMPLEMENT_REFLECTION(RichFrameParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;


void RichFrameParserImpl::PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target)
{
    FrameParserImpl::PreParse(styleNode, target);

    NRichFrame* targetFrame = dynamic_cast<NRichFrame*>(targetObj_);
    text_ = targetFrame->GetRichText();
}

void RichFrameParserImpl::PostParse()
{
    FrameParserImpl::PostParse();
    text_ = NULL;
}

bool RichFrameParserImpl::SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue)
{
    if(FrameParserImpl::SetAttr(attrName, attrValue))
        return true;

    NRichFrame* targetFrame = dynamic_cast<NRichFrame*>(targetObj_);

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
