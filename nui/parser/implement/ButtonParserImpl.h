#pragma once

#include "FrameParserImpl.h"


class ButtonParserImpl : public FrameParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("buttonparser"))
public:
    ButtonParserImpl();

    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
