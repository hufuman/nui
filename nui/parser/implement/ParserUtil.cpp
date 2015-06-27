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

    bool ParseBool(LPCTSTR data)
    {
        if(_tcscmp(data, _T("true")) == 0)
            return true;
        return false;
    }

    INT32 ParseInt32(LPCTSTR data)
    {
        return _ttoi(data);
    }

    INT64 ParseInt64(LPCTSTR data)
    {
        return _tstoi64(data);
    }

    DWORD ParseDword(LPCTSTR data)
    {
        DWORD value = 0;
        _stscanf(data, TEXT("%u"), &value);
        return value;
    }

    bool ParsePoint(LPCTSTR data, nui::Base::NPoint& point)
    {
        return _stscanf(data, TEXT("%d,%d"), &point.X, &point.Y) == 2;
    }

    bool ParseSize(LPCTSTR data, nui::Base::NSize& size)
    {
        return _stscanf(data, TEXT("%d,%d"), &size.Width, &size.Height) == 2;
    }

    bool ParseRect(LPCTSTR data, nui::Base::NRect& rect)
    {
        return _stscanf(data, TEXT("%d,%d,%d,%d"), &rect.Left, &rect.Top, &rect.Right, &rect.Bottom) == 4;
    }

}