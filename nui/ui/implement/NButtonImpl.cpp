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
            SetForeDraw(loader->LoadImage(_T("@skin:common\\button.png")));
        }
        NButton::~NButton()
        {
        }
    }
}
