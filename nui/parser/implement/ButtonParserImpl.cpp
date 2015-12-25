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

void ButtonParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    FrameParserImpl::FillAttr(targetObj, styleNode);
}
