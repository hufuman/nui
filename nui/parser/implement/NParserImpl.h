#pragma once


#include "BaseParser.h"
#include "../NParser.h"
#include "../../base/NAutoPtr.h"

class NParserImpl : public nui::Parser::NParser
{
public:
    virtual nui::Base::NAutoPtr<nui::Base::NBaseObj> Parse(nui::Base::NBaseObj* parentObj, LPCTSTR packFilePath);
    virtual nui::Base::NAutoPtr<nui::Data::NDataReader> FindStyleNode(LPCTSTR packFilePath);

private:
    bool GetStyleParam(LPCTSTR packFilePath, nui::Base::NString& filePath, nui::Base::NString& styleName);
    nui::Base::NAutoPtr<nui::Data::NDataReader> FindStyleNode(nui::Base::NAutoPtr<nui::Data::NDataReader> dataReader, const nui::Base::NString& styleName);
    nui::Base::NAutoPtr<nui::Data::NDataReader> LoadPackFile(LPCTSTR packFilePath, nui::Base::NString& styleName);
};
