#include "stdafx.h"
#include "XmlDataReader.h"

using namespace NUI;
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
    document_ = new TiXmlDocument();
    document_->Parse(utf8Data.c_str());
    if(!document_->Error()
        && (root_ = document_->RootElement()) != NULL)
    {
        return true;
    }
    Clear();
    return false;
}

bool XmlDataReader::ReadValue(LPCTSTR valueName, NString& value)
{
    if(!IsValid())
        return false;
    return ElementToString(root_, valueName, value);
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

    XmlDataReader* reader = new XmlDataReader();
    reader->parent_ = this;
    reader->root_ = element;
    this->AddRef();

    if(value != NULL)
        NSafeRelease(value);
    value = dynamic_cast<NDataReader*>(reader);
    return true;
}

bool XmlDataReader::ReadPath(size_t index, LPCTSTR path, LPCTSTR valueName, NString& value)
{
    NString token;
    size_t position = 0;
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

    XmlDataReader* reader = new XmlDataReader();
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
    NFree(document_);
    root_ = NULL;
}

bool XmlDataReader::ElementToString(TiXmlElement* element, LPCTSTR valueName, NString& value)
{
    std::string strValueName = t2utf8(valueName);
    const char* data = element->Attribute(strValueName.c_str());
    const char* p = element->Value();
    if(data == NULL)
        return false;
    value = utf82t(data).c_str();
    return true;
}
