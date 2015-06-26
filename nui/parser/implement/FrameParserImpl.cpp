#include "stdafx.h"
#include "FrameParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NFrame.h"

IMPLEMENT_REFLECTION(FrameParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

bool FrameParserImpl::SetAttr(LPCTSTR attrName, LPCTSTR attrValue)
{
    UNREFERENCED_PARAMETER(attrName);
    UNREFERENCED_PARAMETER(attrValue);
    return true;
}

void FrameParserImpl::PostParse()
{
    NAutoPtr<NFrame> targetFrame = dynamic_cast<NFrame*>(targetObj_);

    for(int i=0; ; ++ i)
    {
        NAutoPtr<NDataReader> childNode;
        if(!styleNode_->ReadNode(i, childNode))
            break;
        NAutoPtr<NFrame> frame = dynamic_cast<NFrame*>((NBaseObj*)ParserUtil::LoadObj(childNode));
        targetFrame->AddChild(frame);
    }
}
