#include "stdafx.h"
#include "NTileLayoutArranger.h"

#undef min
#undef max

namespace nui
{
    namespace Ui
    {
        void NTileLayoutArranger::RelayoutChild(NFrameBase* child, NLayoutArrangerParam& param) const
        {
            if(!IfCouldLayout(child))
                return;

            if(param.size_.Width + child->GetMargin().Left + child->GetRect().Width() + child->GetMargin().Right > param.frameSize_.Width)
            {
                param.size_.Width = 0;
                param.size_.Height += param.data_;
            }

            param.size_.Width += child->GetMargin().Left;
            child->SetPos(param.size_.Width, param.size_.Height + child->GetMargin().Top);
            param.size_.Width += child->GetRect().Width() + child->GetMargin().Right;

            // store max height in current line
            param.data_ = std::max(param.data_, child->GetMargin().Top + child->GetRect().Height()  + child->GetMargin().Bottom);
            param.maxSize_.Width = std::max(param.maxSize_.Width, param.size_.Width);
            param.maxSize_.Height = std::max(param.maxSize_.Height, param.size_.Height + param.data_);
        }

        bool NTileLayoutArranger::IfCouldLayout(const NFrameBase* const &child) const
        {
            UINT flags = child->GetLayout();
            return flags == 0;
        }
    }
}