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
        newObj_ = false;
        creator_ = NULL;
    }
    virtual ~NBaseParser() {}

    virtual nui::Base::NBaseObj* Alloc()
    {
        newObj_ = true;
        return creator_();
    }

    void SetNewObj(bool newObj)
    {
        newObj_ = newObj;
    }

    virtual bool IsCorrectObj(nui::Base::NBaseObj* targetObj) = 0;
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
    bool newObj_;
    ObjectCreator creator_;
};
