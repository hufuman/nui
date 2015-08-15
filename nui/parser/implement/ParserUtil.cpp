#include "stdafx.h"
#include "ParserUtil.h"

using namespace nui;
using namespace Base;
using namespace Data;

namespace ParserUtil
{
    NAutoPtr<NBaseObj> LoadObj(nui::Base::NBaseObj* parentObj, Base::NAutoPtr<Data::NDataReader> styleNode)
    {
        NString objName = styleNode->GetNodeName();
        objName.MakeLower();

        NAutoPtr<NBaseObj> obj = NReflect::GetInstance().Create(objName, MemToolParam);
        if(obj == NULL)
            return NULL;

        obj->Release();

        NString parserName = objName + _T("parser");
        NAutoPtr<NBaseParser> parser = dynamic_cast<NBaseParser*>(NReflect::GetInstance().Create(parserName, MemToolParam));
        NAssertError(parser != NULL, _T("Parser not found, Name: %s"), parserName.GetData());

        parser->PreParse(styleNode, obj);

        parser->Create(parentObj);

        parser->FillAttr();
        parser->PostParse();
        parser->Release();

        return obj;
    }

    bool FillObjectAttr(NAutoPtr<NBaseParser> parser, NAutoPtr<NBaseObj> baseObj, NAutoPtr<nui::Data::NDataReader> styleNode)
    {
        parser->FillAttr();
        parser->PostParse();
        return true;
    }
}