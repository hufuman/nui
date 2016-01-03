#pragma once

#include "FrameParserImpl.h"


class LabelParserImpl : public FrameParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("labelparser"))
public:
    LabelParserImpl();

    virtual bool IsCorrectObj(nui::Base::NBaseObj* targetObj);
    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
};
