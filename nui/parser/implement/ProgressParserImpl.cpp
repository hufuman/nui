#include "stdafx.h"
#include "ProgressParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NProgress.h"

IMPLEMENT_REFLECTION(ProgressParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

ProgressParserImpl::ProgressParserImpl()
{
	creator_ = &NBaseParser::BaseCreator<NProgress>;
}

bool ProgressParserImpl::IsCorrectObj(nui::Base::NBaseObj* targetObj)
{
	return targetObj != NULL && dynamic_cast<NProgress*>(targetObj) != NULL;
}

void ProgressParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
	FrameParserImpl::FillAttr(targetObj, styleNode);
}
