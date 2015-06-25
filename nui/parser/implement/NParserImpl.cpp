#include "stdafx.h"
#include "NParserImpl.h"

using namespace nui;
using namespace Base;
using namespace Data;

IMPLEMENT_REFLECTION_EX(NParserImpl, NReflect::Singleton);

NAutoPtr<NBaseObj> NParserImpl::Parse(LPCTSTR packFilePath)
{
    NString filePath;
    NString styleName;

    if(!GetStyleParam(packFilePath, filePath, styleName))
        return NULL;

    // Find Resource
    NInstPtr<NBuffer> buffer(MemToolParam);
    NInstPtr<NFileSystem> fs(MemToolParam);
    if(!fs->LoadFile(filePath.GetData(), buffer))
        return NULL;

    // Load Xml
    NAutoPtr<NDataReader> reader = CreateDataReader(ReaderXml);
    if(!reader->ParseUtf8(static_cast<const char*>(buffer->GetBuffer()), buffer->GetSize()))
        return NULL;

    return LoadObj(reader, styleName);
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

NBaseObj* NParserImpl::LoadObj(Base::NAutoPtr<Data::NDataReader> dataReader, LPCTSTR styleName)
{
    UNREFERENCED_PARAMETER(styleName);
    UNREFERENCED_PARAMETER(dataReader);

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
    if(styleNode == NULL)
        return NULL;

    NString objName = styleNode->GetNodeName();
    objName.MakeLower();

    NBaseObj* obj = NReflect::GetInstance().Create(objName, MemToolParam);
    if(obj == NULL)
        return NULL;

    return obj;
}