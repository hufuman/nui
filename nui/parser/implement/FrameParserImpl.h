#pragma once

#include "FrameBaseParserImpl.h"


class FrameParserImpl : public FrameBaseParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("frameparser"))
public:
    FrameParserImpl();
    virtual void Create(nui::Base::NBaseObj* parentObj);
    virtual void PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target);
    virtual void PostParse();
    virtual void FillAttr();

protected:
    nui::Ui::NText* text_;
};
