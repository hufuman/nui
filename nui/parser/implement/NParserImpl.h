#pragma once


#include "BaseParser.h"
#include "../NParser.h"
#include "../../base/NAutoPtr.h"

class NParserImpl : public nui::Parser::NParser
{
public:
    virtual nui::Base::NAutoPtr<nui::Base::NBaseObj> Parse(nui::Base::NBaseObj* parentObj, LPCTSTR stylePath);
    virtual bool ApplyStyle(nui::Base::NBaseObj* targetObj, LPCTSTR stylePath);
    virtual nui::Base::NAutoPtr<nui::Data::NDataReader> FindStyleNode(LPCTSTR stylePath);

private:
    bool GetStyleParam(LPCTSTR stylePath, nui::Base::NString& filePath, nui::Base::NString& styleName);
    nui::Base::NAutoPtr<nui::Data::NDataReader> LoadPackFile(LPCTSTR filePath);
};
