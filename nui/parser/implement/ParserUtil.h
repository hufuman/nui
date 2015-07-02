#pragma once


#include "BaseParser.h"
#include "../../base/NAutoPtr.h"

namespace ParserUtil
{
    nui::Base::NAutoPtr<nui::Base::NBaseObj> LoadObj(nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode);
    bool FillObjectAttr(nui::Base::NAutoPtr<NBaseParser> parser,
        nui::Base::NAutoPtr<nui::Base::NBaseObj> baseObj,
        nui::Base::NAutoPtr<nui::Data::NDataReader> styleNode);

    bool ParseBool(LPCTSTR data);
    INT32 ParseInt32(LPCTSTR data);
    INT64 ParseInt64(LPCTSTR data);
    DWORD ParseDword(LPCTSTR data);
    bool ParsePoint(LPCTSTR data, nui::Base::NPoint& point);
    bool ParseSize(LPCTSTR data, nui::Base::NSize& size);
    bool ParseRect(LPCTSTR data, nui::Base::NRect& rect);
    nui::Ui::ArgbColor ParseArgb(LPCTSTR data);
}
