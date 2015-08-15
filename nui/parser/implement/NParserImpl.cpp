#include "stdafx.h"
#include "NParserImpl.h"

#include "ParserUtil.h"

using namespace nui;
using namespace Base;
using namespace Data;

IMPLEMENT_REFLECTION_EX(NParserImpl, NReflect::Singleton);

NAutoPtr<NBaseObj> NParserImpl::Parse(Ui::NFrame* parentFrame, LPCTSTR packFilePath)
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

    NAutoPtr<NDataReader> styleNode = FindStyleNode(reader, styleName);

    return ParserUtil::LoadObj(parentFrame, styleNode);
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

NAutoPtr<NDataReader> NParserImpl::FindStyleNode(Base::NAutoPtr<Data::NDataReader> dataReader, LPCTSTR styleName)
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
