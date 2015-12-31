#pragma once

#include "ButtonParserImpl.h"


class CheckBoxParserImpl : public ButtonParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("checkboxparser"))
public:
    CheckBoxParserImpl();

    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
