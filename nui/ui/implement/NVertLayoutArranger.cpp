#include "stdafx.h"
#include "NVertLayoutArranger.h"

#undef min
#undef max

namespace nui
{
    namespace Ui
    {

        bool NVertLayoutArranger::IfCouldLayout(const NFrameBase* const &child) const
        {
            UINT flags = child->GetLayout();
            return ((flags & NFrameBase::LayoutTop) != NFrameBase::LayoutTop)
                || ((flags & NFrameBase::LayoutBottom) != NFrameBase::LayoutBottom)
                || ((flags & NFrameBase::LayoutVCenter) != NFrameBase::LayoutVCenter)
                || ((flags & NFrameBase::LayoutVFill) != NFrameBase::LayoutVFill);
        }

        void NVertLayoutArranger::RelayoutChild(NFrameBase* child, NLayoutArrangerParam& param) const
        {
            if(!IfCouldLayout(child))
                return;

            param.maxSize_.Height += child->GetMargin().Top;
            child->SetPos(child->GetRect().Left, param.maxSize_.Height);
            param.maxSize_.Width = std::max(child->GetRect().Width(), param.maxSize_.Width);
            param.maxSize_.Height += child->GetMargin().Top + child->GetRect().Height() + child->GetMargin().Bottom;
        }
    }
}