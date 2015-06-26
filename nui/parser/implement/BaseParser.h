#pragma once

#include "../../base/BaseObj.h"


class NBaseParser : public nui::Base::NBaseObj
{
public:
    NBaseParser()
    {
        targetObj_ = NULL;
    }

    virtual ~NBaseParser()
    {
        targetObj_ = NULL;
        styleNode_ = NULL;
    }

    void PreParse(nui::Data::NDataReader* styleNode, nui::Base::NBaseObj* target)
    {
        targetObj_ = target;
        styleNode_ = styleNode;
    }

    virtual void PostParse()
    {
        targetObj_ = NULL;
        styleNode_ = NULL;
    }

    virtual bool SetAttr(LPCTSTR attrName, LPCTSTR attrValue) = 0;

protected:
    nui::Base::NBaseObj* targetObj_;
    nui::Data::NDataReader* styleNode_;
};
