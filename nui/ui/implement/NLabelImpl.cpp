#include "stdafx.h"
#include "../NLabel.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NLabel);

        NLabel::NLabel()
        {
        }

        NLabel::~NLabel()
        {
        }

        void NLabel::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:label"));
#endif  // _NO_NUI_PARSER_
        }
    }
}
