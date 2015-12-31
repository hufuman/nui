#include "stdafx.h"
#include "LabelParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NLabel.h"

IMPLEMENT_REFLECTION(LabelParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

LabelParserImpl::LabelParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NLabel>;
}

void LabelParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    FrameParserImpl::FillAttr(targetObj, styleNode);
}
