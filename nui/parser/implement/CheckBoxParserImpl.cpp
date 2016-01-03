#include "stdafx.h"
#include "CheckBoxParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NCheckBox.h"

IMPLEMENT_REFLECTION(CheckBoxParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

CheckBoxParserImpl::CheckBoxParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NCheckBox>;
}

bool CheckBoxParserImpl::IsCorrectObj(nui::Base::NBaseObj* targetObj)
{
    return targetObj != NULL && dynamic_cast<NCheckBox*>(targetObj) != NULL;
}

void CheckBoxParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    ButtonParserImpl::FillAttr(targetObj, styleNode);

    bool bFlag;
    NCheckBox* targetButton = dynamic_cast<NCheckBox*>(targetObj);

    if(styleNode->ReadValue(_T("scaleImage"), bFlag))
        targetButton->SetScaleImage(bFlag);
}
