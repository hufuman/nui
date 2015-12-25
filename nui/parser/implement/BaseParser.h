#pragma once

#include "../../base/BaseObj.h"


class NBaseParser : public nui::Base::NBaseObj
{
public:
    typedef nui::Base::NBaseObj* (*ObjectCreator)();

    template <typename TObjType>
    static nui::Base::NBaseObj* BaseCreator()
    {
        Base::NInstPtr<TObjType> targetObj(MemToolParam);
        if(targetObj)
        {
            targetObj->AddRef();
        }
        return targetObj;
    }

    NBaseParser()
    {
        creator_ = NULL;
    }
    virtual ~NBaseParser() {}

    virtual nui::Base::NBaseObj* Alloc()
    {
        return creator_();
    }
    virtual void Create(nui::Base::NBaseObj* parentObj, nui::Base::NBaseObj* targetObj) = 0;
    virtual void FillAttr(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode) = 0;

    virtual void PreParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
    {
        UNREFERENCED_PARAMETER(targetObj);
        UNREFERENCED_PARAMETER(styleNode);
    }

    virtual void PostParse(nui::Base::NBaseObj* targetObj, nui::Data::NDataReader* styleNode)
    {
        UNREFERENCED_PARAMETER(targetObj);
        UNREFERENCED_PARAMETER(styleNode);
    }

protected:
    ObjectCreator creator_;
};
