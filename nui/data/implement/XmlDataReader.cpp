#include "stdafx.h"
#include "XmlDataReader.h"

using namespace nui;
using namespace Base;
using namespace Data;

XmlDataReader::XmlDataReader(void)
{
    parent_ = NULL;
    document_ = NULL;
    root_ = NULL;
}

XmlDataReader::~XmlDataReader(void)
{
    Clear();
}

bool XmlDataReader::ParseUtf8(const char* data, size_t length)
{
    Clear();

    std::string utf8Data;
    utf8Data.assign(data, length);
    document_ = NNew(TiXmlDocument);
    document_->Parse(utf8Data.c_str());
    if(!document_->Error()
        && (root_ = document_->RootElement()) != NULL)
    {
        return true;
    }
#ifdef _DEBUG
    if(document_->Error())
    {
        nui::Base::NString errorDesc = nui::Data::a2t(document_->ErrorDesc());
        int row = document_->ErrorRow();
        int column = document_->ErrorCol();
        NAssertError(false, _T("Failed to parse xml\r\n\trow: %d, col: %d\r\n\tReason: %s"), row, column, errorDesc.GetData());
    }
    else
    {
        NAssertError(false, _T("No root element found"));
    }
#endif // _DEBUG
    Clear();
    return false;
}

bool XmlDataReader::ReadValue(LPCTSTR valueName, NString& value)
{
    if(!IsValid())
        return false;
    return ElementToString(root_, valueName, value);
}

bool XmlDataReader::ReadValue(int index, nui::Base::NString& name, nui::Base::NString& value)
{
    if(!IsValid())
        return false;

    TiXmlAttribute* attr = root_->FirstAttribute();
    for(int i=0; attr != NULL && i<index; ++ i)
    {
        attr = attr->Next();
    }
    if(attr == NULL)
        return false;

    const char* attrName = attr->Name();
    const char* attrValue = attr->Value();
    if(attrValue == NULL || attrName == NULL)
        return false;

    name = utf82t(attrName);
    value = utf82t(attrValue);
    return true;
}

bool XmlDataReader::ReadPath(LPCTSTR path, LPCTSTR valueName, NString& value)
{
    return ReadPath(0, path, valueName, value);
}

bool XmlDataReader::ReadNode(LPCTSTR nodeName, NDataReader*& value)
{
    std::string strNodeName = t2utf8(nodeName);
    TiXmlElement* element = root_->FirstChildElement(strNodeName.c_str());
    if(element == NULL)
        return false;

    XmlDataReader* reader = NNew(XmlDataReader);
    reader->AddRef();
    reader->parent_ = this;
    reader->root_ = element;
    this->AddRef();

    if(value != NULL)
        NSafeRelease(value);
    value = dynamic_cast<NDataReader*>(reader);
    return true;
}

bool XmlDataReader::ReadNodeByPath(LPCTSTR path, NDataReader*& value)
{
    NString token;
    int position = 0;
    NString strPath(path);
    TiXmlElement* element = root_;
    std::string nodeName;

    while(element != NULL && strPath.Tokenize(position, TEXT("\\"), false, token))
    {
        nodeName = t2utf8(token.GetData());
        element = element->FirstChildElement(nodeName.c_str());
    }
    if(element == NULL)
        return false;

    XmlDataReader* reader = NNew(XmlDataReader);
    reader->AddRef();
    reader->parent_ = this;
    reader->root_ = element;
    this->AddRef();

    if(value != NULL)
        NSafeRelease(value);
    value = dynamic_cast<NDataReader*>(reader);
    return true;
}

nui::Base::NString XmlDataReader::GetNodeName() const
{
    const char* name = root_->Value();
    if(name == NULL)
        return _T("");
    return utf82t(name);
}

bool XmlDataReader::ReadPath(size_t index, LPCTSTR path, LPCTSTR valueName, NString& value)
{
    NString token;
    int position = 0;
    NString strPath(path);
    TiXmlElement* element = root_;
    std::string nodeName;

    while(element != NULL && strPath.Tokenize(position, TEXT("\\"), false, token))
    {
        nodeName = t2utf8(token.GetData());
        element = element->FirstChildElement(nodeName.c_str());
    }
    if(element == NULL)
        return false;

    for(size_t i=0; element != NULL && i<index; ++ i)
    {
        element = element->NextSiblingElement(nodeName.c_str());
    }
    if(element == NULL)
        return false;

    return ElementToString(element, valueName, value);
}

bool XmlDataReader::ReadNode(size_t index, LPCTSTR nodeName, NDataReader*& value)
{
    std::string strNodeName = t2utf8(nodeName);
    TiXmlElement* element = root_->FirstChildElement(strNodeName.c_str());
    for(size_t i=0; element !=NULL && i<index; ++ i)
    {
        element = element->NextSiblingElement(strNodeName.c_str());
    }
    if(element == NULL)
        return false;

    XmlDataReader* reader = NNew(XmlDataReader);
    reader->AddRef();
    reader->parent_ = this;
    reader->root_ = element;
    this->AddRef();

    if(value != NULL)
        NSafeRelease(value);
    value = dynamic_cast<NDataReader*>(reader);
    return true;
}

bool XmlDataReader::ReadNode(size_t index, NDataReader*& value)
{
    TiXmlElement* element = root_->FirstChildElement();
    for(size_t i=0; element !=NULL && i<index; ++ i)
    {
        element = element->NextSiblingElement();
    }
    if(element == NULL)
        return false;

    XmlDataReader* reader = NNew(XmlDataReader);
    reader->AddRef();
    reader->parent_ = this;
    reader->root_ = element;
    this->AddRef();

    if(value != NULL)
        NSafeRelease(value);
    value = dynamic_cast<NDataReader*>(reader);
    return true;
}

void XmlDataReader::Clear()
{
    NSafeRelease(parent_);
    NDelete(document_);
    root_ = NULL;
}

bool XmlDataReader::ElementToString(TiXmlElement* element, LPCTSTR valueName, NString& value)
{
    std::string strValueName = t2utf8(valueName);
    const char* data = element->Attribute(strValueName.c_str());
    if(data == NULL)
        return false;
    value = utf82t(data);
    return true;
}
