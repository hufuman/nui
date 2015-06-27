#pragma once

#include "FrameParserImpl.h"


class RichFrameParserImpl : public FrameParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("frameparser"))
public:
    virtual bool SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue);
};
