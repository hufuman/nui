#pragma once

#include "FrameParserImpl.h"


class LayoutParserImpl : public FrameParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("layoutparser"))
public:
    LayoutParserImpl();

    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
    virtual void Create(nui::Base::NBaseObj* parentObj, nui::Base::NBaseObj* targetObj);
};
