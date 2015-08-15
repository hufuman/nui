#pragma once

#include "../../base/BaseObj.h"


class NBaseParser : public nui::Base::NBaseObj
{
public:
    NBaseParser()
    {
        targetObj_ = NULL;
        styleNode_ = NULL;
    }

    virtual ~NBaseParser()
    {
        targetObj_ = NULL;
        styleNode_ = NULL;
    }

    virtual void Create(nui::Base::NBaseObj* parentObj)
    {
        UNREFERENCED_PARAMETER(parentObj);
    }

    // return false if not value not set, otherwise true;
    virtual void FillAttr() = 0;

    virtual void PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target)
    {
        targetObj_ = target;
        styleNode_ = styleNode;
    }
    virtual void PostParse()
    {}

protected:
    nui::Base::NBaseObj* targetObj_;
    nui::Data::NDataReader* styleNode_;
};
