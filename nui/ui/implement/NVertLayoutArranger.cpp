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

            const Base::NRect& margin = child->GetMargin();
            param.maxSize_.Height += margin.Top;
            child->SetPos(margin.Left, param.maxSize_.Height);
            param.maxSize_.Height += child->GetRect().Height() + margin.Bottom;
            param.maxSize_.Width = std::max(child->GetRect().Width() + margin.Left + margin.Right, param.maxSize_.Width);
        }
    }
}