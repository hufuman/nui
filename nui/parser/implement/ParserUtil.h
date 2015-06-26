#pragma once


#include "BaseParser.h"
#include "../../base/NAutoPtr.h"

namespace ParserUtil
{
    nui::Base::NAutoPtr<nui::Base::NBaseObj> LoadObj(nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode);
    bool FillObjectAttr(nui::Base::NAutoPtr<NBaseParser> parser,
        nui::Base::NAutoPtr<nui::Base::NBaseObj> baseObj,
        nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode);
}
