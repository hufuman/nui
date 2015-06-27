#include "stdafx.h"
#include "ButtonParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NButton.h"

IMPLEMENT_REFLECTION(ButtonParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

bool ButtonParserImpl::SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue)
{
    if(FrameParserImpl::SetAttr(attrName, attrValue))
        return true;
    UNREFERENCED_PARAMETER(attrName);
    UNREFERENCED_PARAMETER(attrValue);
    return false;
}
