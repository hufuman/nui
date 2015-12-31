#include "stdafx.h"
#include "NParserImpl.h"

#include "ParserUtil.h"

using namespace nui;
using namespace Base;
using namespace Data;

IMPLEMENT_REFLECTION_EX(NParserImpl, NReflect::Singleton);

NAutoPtr<NBaseObj> NParserImpl::Parse(Base::NBaseObj* parentObj, LPCTSTR packFilePath)
{
    NString styleName;
    NAutoPtr<NDataReader> reader = LoadPackFile(packFilePath, styleName);
    if(!reader)
        return NULL;

    NAutoPtr<NDataReader> styleNode = FindStyleNode(reader, styleName);
    if(!styleNode)
        return NULL;

    return ParserUtil::LoadObj(parentObj, styleNode);
}

bool NParserImpl::ApplyStyle(nui::Base::NBaseObj* targetObj, LPCTSTR styleName)
{
    nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode = FindStyleNode(styleName);
    if(!styleNode)
        return false;

    return ParserUtil::ApplyStyle(targetObj, styleNode);
}

nui::Base::NAutoPtr<nui::Data::NDataReader> NParserImpl::FindStyleNode(LPCTSTR packFilePath)
{
    NString styleName;
    nui::Base::NAutoPtr<nui::Data::NDataReader> reader = LoadPackFile(packFilePath, styleName);
    if(!reader)
        return NULL;

    return FindStyleNode(reader, styleName);
}

bool NParserImpl::GetStyleParam(LPCTSTR packFilePath, NString& filePath, NString& styleName)
{
    // Normalize path
    LPCTSTR separator = _tcschr(packFilePath, _T(':'));
    if(separator == NULL)
        return false;

    styleName = separator + 1;

    if(packFilePath[0] == _T('@'))
    {
        NString tmpPath;
        tmpPath.Assign(packFilePath, separator - packFilePath);
        filePath.Format(_T("@style:%s.xml"), tmpPath.GetData() + 1);
    }
    else
    {
        filePath.Assign(packFilePath, separator - packFilePath);
    }

    return true;
}

NAutoPtr<NDataReader> NParserImpl::FindStyleNode(Base::NAutoPtr<Data::NDataReader> dataReader, const Base::NString& styleName)
{
    NAutoPtr<NDataReader> styleNode;
    for(int i=0;; ++i)
    {
        if(!dataReader->ReadNode(i, styleNode))
            return NULL;
        Base::NString name;
        if(styleNode->ReadValue(_T("name"), name) && name == styleName)
            break;
        styleNode = NULL;
    }
    return styleNode;
}

NAutoPtr<NDataReader> NParserImpl::LoadPackFile(LPCTSTR packFilePath, nui::Base::NString& styleName)
{
    NString filePath;

    if(!GetStyleParam(packFilePath, filePath, styleName))
    {
        NAssertError(false, _T("failed to parse PackFilePath: %s"), packFilePath);
        return NULL;
    }

    // Find Resource
    NInstPtr<NBuffer> buffer(MemToolParam);
    NInstPtr<NFileSystem> fs(MemToolParam);
    if(!fs->LoadFile(filePath.GetData(), buffer))
    {
        NAssertError(false, _T("failed to load file: %s"), filePath.GetData());
        return NULL;
    }

    // Load Xml
    NAutoPtr<NDataReader> reader = CreateDataReader(ReaderXml);
    if(!reader->ParseUtf8(static_cast<const char*>(buffer->GetBuffer()), buffer->GetSize()))
    {
        NAssertError(false, _T("failed to parse file: %s"), filePath.GetData());
        return NULL;
    }

    return reader;
}