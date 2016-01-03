#pragma once

#include "CheckBoxParserImpl.h"


class RadioBoxParserImpl : public CheckBoxParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("radioboxparser"))
public:
    RadioBoxParserImpl();

    virtual bool IsCorrectObj(nui::Base::NBaseObj* targetObj);
    virtual void PreParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
