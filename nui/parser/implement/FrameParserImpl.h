#pragma once

#include "BaseParser.h"
#include "../../ui/NFrame.h"


class FrameParserImpl : public NBaseParser
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("frameparser"))
public:
    FrameParserImpl();

    virtual bool IsCorrectObj(nui::Base::NBaseObj* targetObj);
    virtual void Create(nui::Base::NBaseObj* parentObj, nui::Base::NBaseObj* targetObj);
    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
    virtual void PreParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);
    virtual void PostParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode);

private:
    void FillTextAttr(nui::Ui::NFrame* targetFrame, nui::Data::NDataReader* styleNode);
};
