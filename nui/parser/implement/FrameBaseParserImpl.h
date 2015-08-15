#pragma once

#include "BaseParser.h"


class FrameBaseParserImpl : public NBaseParser
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("framebaseparser"))
public:
    virtual void PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target);
    virtual void FillAttr();
    virtual void PostParse();
};
