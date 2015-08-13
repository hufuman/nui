#pragma once

#include "BaseParser.h"


class FrameBaseParserImpl : public NBaseParser
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("framebaseparser"))
public:
    virtual void PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target);
    virtual bool SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue);
    virtual void PostParse();
};
