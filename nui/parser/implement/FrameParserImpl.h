#pragma once

#include "BaseParser.h"


class FrameParserImpl : public NBaseParser
{
    DECLARE_REFLECTION(TEXT("nui"), TEXT("frameparser"))
public:
    virtual bool SetAttr(LPCTSTR attrName, LPCTSTR attrValue);
    virtual void PostParse();
};
