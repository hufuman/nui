#include "stdafx.h"
#include "NTileLayoutArranger.h"

#undef min
#undef max

namespace nui
{
    namespace Ui
    {
        void NTileLayoutArranger::RelayoutChild(NFrameBase* child, Base::NSize& size) const
        {
            UNREFERENCED_PARAMETER(child);
            UNREFERENCED_PARAMETER(size);
        }

        bool NTileLayoutArranger::IfCouldLayout(const NFrameBase* const &child) const
        {
            UNREFERENCED_PARAMETER(child);
            return true;
        }
    }
}