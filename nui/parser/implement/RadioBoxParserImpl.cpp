#include "stdafx.h"
#include "RadioBoxParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NRadioBox.h"

IMPLEMENT_REFLECTION(RadioBoxParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

RadioBoxParserImpl::RadioBoxParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NRadioBox>;
}

void RadioBoxParserImpl::PreParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    CheckBoxParserImpl::PreParse(targetObj, styleNode);

    NString tmpString;
    NRadioBox* targetRadioBox = dynamic_cast<NRadioBox*>(targetObj);

    if(styleNode->ReadValue(_T("group"), tmpString))
        targetRadioBox->SetGroup(tmpString);
}
