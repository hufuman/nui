#pragma once


#include "../NDataReader.h"
#include <tinyxml.h>


class XmlDataReader : public nui::Data::NDataReader
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("xmlDataReader"))
public:
    XmlDataReader(void);
    virtual ~XmlDataReader(void);

    virtual bool ParseUtf8(const char* data, size_t length);

    virtual bool ReadValue(LPCTSTR valueName, nui::Base::NString& value);

    virtual bool ReadPath(LPCTSTR path, LPCTSTR valueName, nui::Base::NString& value);
    virtual bool ReadNode(LPCTSTR nodeName, NDataReader*& value);
    virtual bool ReadNodeByPath(LPCTSTR path, NDataReader*& value);

    virtual bool ReadPath(size_t index, LPCTSTR path, LPCTSTR valueName, nui::Base::NString& value);
    virtual bool ReadNode(size_t index, LPCTSTR nodeName, NDataReader*& value);

private:
    bool IsValid() const
    {
        return (root_ != NULL);
    }

    void Clear();

    static bool ElementToString(TiXmlElement* element, LPCTSTR valueName, nui::Base::NString& value);

private:
    XmlDataReader* parent_;
    TiXmlDocument* document_;
    TiXmlElement*  root_;
};
