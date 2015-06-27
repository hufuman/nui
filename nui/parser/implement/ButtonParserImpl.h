#pragma once

#include "FrameParserImpl.h"


class ButtonParserImpl : public FrameParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("buttonparser"))
public:
    virtual bool SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue);
};
