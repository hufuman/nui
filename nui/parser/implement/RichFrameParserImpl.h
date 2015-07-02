#pragma once

#include "FrameParserImpl.h"


class RichFrameParserImpl : public FrameParserImpl
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("richframeparser"))
public:
    virtual void PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target);
    virtual void PostParse();
    virtual bool SetAttr(const nui::Base::NString& attrName, const nui::Base::NString& attrValue);

protected:
    nui::Ui::NText* text_;
};
