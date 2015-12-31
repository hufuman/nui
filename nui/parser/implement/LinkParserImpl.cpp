#include "stdafx.h"
#include "LinkParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NLink.h"

IMPLEMENT_REFLECTION(LinkParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

LinkParserImpl::LinkParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NLink>;
}

void LinkParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    __super::FillAttr(targetObj, styleNode);

    NString tmpString;
    NLink* targetLink = dynamic_cast<NLink*>(targetObj);

    if(styleNode->ReadValue(_T("url"), tmpString))
        targetLink->SetUrl(tmpString);
}
