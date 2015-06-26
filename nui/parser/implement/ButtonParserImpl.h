#pragma once

#include "BaseParser.h"


class ButtonParserImpl : public NBaseParser
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("buttonparser"))
public:
    virtual bool SetAttr(LPCTSTR attrName, LPCTSTR attrValue);
};
