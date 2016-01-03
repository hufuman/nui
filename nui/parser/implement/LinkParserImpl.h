#pragma once

#include "LabelParserImpl.h"


class LinkParserImpl : public LabelParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("linkparser"))
public:
    LinkParserImpl();

    virtual bool IsCorrectObj(nui::Base::NBaseObj* targetObj);
    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
