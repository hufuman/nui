#include "stdafx.h"
#include "RichFrameParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NRichFrame.h"

IMPLEMENT_REFLECTION(RichFrameParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

bool RichFrameParserImpl::SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue)
{
    if(FrameParserImpl::SetAttr(attrName, attrValue))
        return true;

    NRichFrame* targetFrame = dynamic_cast<NRichFrame*>(targetObj_);

    UNREFERENCED_PARAMETER(targetFrame);
    UNREFERENCED_PARAMETER(attrName);
    UNREFERENCED_PARAMETER(attrValue);
    return false;
}
