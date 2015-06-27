#pragma once

#include "BaseParser.h"


class FrameParserImpl : public NBaseParser
{
public:
    virtual bool SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue);
    virtual void PostParse();
};
