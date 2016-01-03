#pragma once


#include "BaseParser.h"
#include "../../base/NAutoPtr.h"

namespace ParserUtil
{
    nui::Base::NAutoPtr<nui::Base::NBaseObj> LoadObj(bool newObj, nui::Base::NBaseObj* parentObj, nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode);
    bool ApplyStyle(nui::Base::NBaseObj* targetObj, nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode);

    nui::Base::NAutoPtr<nui::Ui::NDraw> ParseDraw(const nui::Base::NString& styleName);
    nui::Base::NAutoPtr<nui::Ui::NDraw> ParseDraw(nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode);
    nui::Base::NAutoPtr<nui::Ui::NFont> ParseFont(nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode);
}
