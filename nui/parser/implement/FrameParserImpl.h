#pragma once

#include "BaseParser.h"


class FrameParserImpl : public NBaseParser
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("frameparser"))
public:
    FrameParserImpl();

    virtual void Create(nui::Base::NBaseObj* parentObj, nui::Base::NBaseObj* targetObj);
    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
    virtual void PreParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
    virtual void PostParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
