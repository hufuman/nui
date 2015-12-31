#pragma once

#include "FrameParserImpl.h"


class EditParserImpl : public FrameParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("editparser"))
public:
    EditParserImpl();

    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
