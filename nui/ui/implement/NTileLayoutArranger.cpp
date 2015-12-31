#include "stdafx.h"
#include "NTileLayoutArranger.h"

#undef min
#undef max

namespace nui
{
    namespace Ui
    {

        bool NTileLayoutArranger::IfCouldLayout(const NFrameBase* const &child) const
        {
            if(!child->IsVisible() || child->GetRect().Width() == 0 || child->GetRect().Height() == 0)
                return false;

            UINT flags = child->GetLayout();
            return flags == 0;
        }

        void NTileLayoutArranger::RelayoutChild(NFrameBase* child, NLayoutArrangerParam& param) const
        {
            if(!IfCouldLayout(child))
                return;

            const Base::NRect& margin = child->GetMargin();
            if(param.size_.Width + margin.Left + child->GetRect().Width() + margin.Right > param.frameSize_.Width)
            {
                param.size_.Width = 0;
                param.size_.Height += param.data_;
            }

            param.size_.Width += margin.Left;
            child->SetPos(param.size_.Width, param.size_.Height + margin.Top);
            param.size_.Width += child->GetRect().Width() + margin.Right;

            // store max height in current line
            param.data_ = std::max(param.data_, margin.Top + child->GetRect().Height()  + margin.Bottom);
            param.maxSize_.Width = std::max(param.maxSize_.Width, param.size_.Width);
            param.maxSize_.Height = std::max(param.maxSize_.Height, param.size_.Height + param.data_);
        }
    }
}