#pragma once

#include "LabelParserImpl.h"


class LinkParserImpl : public LabelParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("linkparser"))
public:
    LinkParserImpl();

    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
