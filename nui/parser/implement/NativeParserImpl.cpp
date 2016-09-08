#include "stdafx.h"
#include "NativeParserImpl.h"

#include "ParserUtil.h"
#include "../../ui/NNative.h"

IMPLEMENT_REFLECTION(NativeParserImpl);

using namespace nui;
using namespace Ui;
using namespace Base;
using namespace Data;

NativeParserImpl::NativeParserImpl()
{
    creator_ = &NBaseParser::BaseCreator<NNative>;
}

bool NativeParserImpl::IsCorrectObj(nui::Base::NBaseObj* targetObj)
{
    return targetObj != NULL && dynamic_cast<NNative*>(targetObj) != NULL;
}

void NativeParserImpl::FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
{
    FrameParserImpl::FillAttr(targetObj, styleNode);

    NNative* targetNative = dynamic_cast<NNative*>(targetObj);

    bool tmpBool;

    if(styleNode->ReadValue(_T("destroyOnLostFocus"), tmpBool))
        targetNative->SetDestroyOnLostFocus(tmpBool);
}
