#include "stdafx.h"
#include "ButtonParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NButton.h"

IMPLEMENT_REFLECTION(ButtonParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

ButtonParserImpl::ButtonParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NButton>;
}

bool ButtonParserImpl::IsCorrectObj(nui::Base::NBaseObj* targetObj)
{
    return targetObj != NULL && dynamic_cast<NButton*>(targetObj) != NULL;
}

void ButtonParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    FrameParserImpl::FillAttr(targetObj, styleNode);
}
