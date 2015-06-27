#pragma once

#include "BaseParser.h"


class FrameParserImpl : public NBaseParser
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("frameparser"))
public:
    virtual bool SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue);
    virtual void PostParse();
};
