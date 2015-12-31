#include "stdafx.h"
#include "NParserImpl.h"

#include "ParserUtil.h"

using namespace nui;
using namespace Base;
using namespace Data;

IMPLEMENT_REFLECTION_EX(NParserImpl, NReflect::Singleton);

NAutoPtr<NBaseObj> NParserImpl::Parse(Base::NBaseObj* parentObj, LPCTSTR stylePath)
{
    if(stylePath == NULL)
        return NULL;

    NAutoPtr<NDataReader> styleNode = FindStyleNode(stylePath);
    if(!styleNode)
        return NULL;

    return ParserUtil::LoadObj(parentObj, styleNode);
}

bool NParserImpl::ApplyStyle(nui::Base::NBaseObj* targetObj, LPCTSTR stylePath)
{
    nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode = FindStyleNode(stylePath);
    if(!styleNode)
        return false;

    return ParserUtil::ApplyStyle(targetObj, styleNode);
}

nui::Base::NAutoPtr<nui::Data::NDataReader> NParserImpl::FindStyleNode(LPCTSTR stylePathParam)
{
    NAutoPtr<NDataReader> styleNode;
    NString filePath;
    NString styleName;

    NString stylePath(stylePathParam);
    // in case of cycle reference
    for(int refCount=0; refCount<5; ++ refCount)
    {
        if(!GetStyleParam(stylePath, filePath, styleName) || filePath.IsEmpty())
            return NULL;

        nui::Base::NAutoPtr<nui::Data::NDataReader> dataReader = LoadPackFile(filePath);
        if(!dataReader)
            return NULL;

        if(styleName.IsEmpty())
        {
            NString tmpString;
            if(dataReader->ReadValue(_T("default"), tmpString) && !tmpString.IsEmpty())
            {
                stylePath = tmpString;
                continue;
            }
        }

        for(int i=0;; ++i)
        {
            if(!dataReader->ReadNode(i, styleNode))
                return NULL;
            if(styleName.IsEmpty())
                break;
            Base::NString name;
            if(styleNode->ReadValue(_T("name"), name) && name == styleName)
                break;
            styleNode = NULL;
        }
    }

    return styleNode;
}

bool NParserImpl::GetStyleParam(LPCTSTR stylePath, NString& filePath, NString& styleName)
{
    // Normalize path
    LPCTSTR separator = _tcschr(stylePath, _T(':'));

    if(separator == NULL)
    {
        if(stylePath[0] == _T('@'))
        {
            filePath.Format(_T("@style:%s.xml"), stylePath + 1);
        }
        else
        {
            styleName = stylePath;
        }
        return true;
    }

    if(stylePath[0] != _T('@'))
        return false;

    styleName = separator + 1;

    filePath.Assign(stylePath, separator - stylePath);
    filePath.Format(_T("@style:%s.xml"), filePath.GetData() + 1);
    return true;
}

NAutoPtr<NDataReader> NParserImpl::LoadPackFile(LPCTSTR filePath)
{
    // Find Resource
    NInstPtr<NBuffer> buffer(MemToolParam);
    NInstPtr<NFileSystem> fs(MemToolParam);
    if(!fs->LoadFile(filePath, buffer))
    {
        NAssertError(false, _T("failed to load file: %s"), filePath);
        return NULL;
    }

    // Load Xml
    NAutoPtr<NDataReader> reader = CreateDataReader(ReaderXml);
    if(!reader->ParseUtf8(static_cast<const char*>(buffer->GetBuffer()), buffer->GetSize()))
    {
        NAssertError(false, _T("failed to parse file: %s"), filePath);
        return NULL;
    }

    return reader;
}