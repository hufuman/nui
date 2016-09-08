#include "stdafx.h"
#include "EditParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NEdit.h"

IMPLEMENT_REFLECTION(EditParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

EditParserImpl::EditParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NEdit>;
}

bool EditParserImpl::IsCorrectObj(nui::Base::NBaseObj* targetObj)
{
    return targetObj != NULL && dynamic_cast<NEdit*>(targetObj) != NULL;
}

void EditParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    NativeParserImpl::FillAttr(targetObj, styleNode);

    NEdit* targetEdit = dynamic_cast<NEdit*>(targetObj);

    bool tmpBool;
    NString tmpString;

    if(styleNode->ReadValue(_T("readOnly"), tmpBool))
        targetEdit->SetReadOnly(tmpBool);

    if(styleNode->ReadValue(_T("hintText"), tmpString))
        targetEdit->SetHintText(tmpString);
}
