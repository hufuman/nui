#include "stdafx.h"
#include "../NButton.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NButton);

        NButton::NButton()
        {
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagCanHover, true);
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            SetBkgDraw(loader->LoadImage(_T("@skin:common\\button.png")));
        }

        NButton::~NButton()
        {
        }

        void NButton::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:button"));
#endif  // _NO_NUI_PARSER_
        }
    }
}
