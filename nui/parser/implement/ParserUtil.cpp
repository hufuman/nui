#include "stdafx.h"
#include "ParserUtil.h"

using namespace nui;
using namespace Base;
using namespace Data;

namespace ParserUtil
{
    NAutoPtr<NBaseObj> LoadObj(Base::NAutoPtr<Data::NDataReader> styleNode)
    {
        NString objName = styleNode->GetNodeName();
        objName.MakeLower();

        NAutoPtr<NBaseObj> obj = NReflect::GetInstance().Create(objName, MemToolParam);
        if(obj == NULL)
            return NULL;

        obj->Release();

        NString parserName = objName + _T("parser");
        NAutoPtr<NBaseParser> parser = dynamic_cast<NBaseParser*>(NReflect::GetInstance().Create(parserName, MemToolParam));
        if(!FillObjectAttr(parser, obj,styleNode))
            obj = NULL;
        parser->Release();

        return obj;
    }

    bool FillObjectAttr(NAutoPtr<NBaseParser> parser, NAutoPtr<NBaseObj> baseObj, NAutoPtr<nui::Data::NDataReader> styleNode)
    {
        UNREFERENCED_PARAMETER(baseObj);

        parser->PreParse(styleNode, baseObj);
        NString attrName;
        NString attrValue;
        for(int i=0; ; ++ i)
        {
            if(!styleNode->ReadValue(i, attrName, attrValue))
                break;
            parser->SetAttr(attrName.GetData(), attrValue.GetData());
        }
        parser->PostParse();
        return true;
    }

}