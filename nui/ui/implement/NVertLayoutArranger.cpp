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

        void NVertLayoutArranger::RelayoutChild(NFrameBase* child, Base::NSize& size) const
        {
            if(!IfCouldLayout(child))
                return;

            size.Height += child->GetMargin().Top;
            child->SetPos(child->GetRect().Left, size.Height);
            size.Width = std::max(child->GetRect().Width(), size.Width);
            size.Height += child->GetMargin().Top + child->GetRect().Height() + child->GetMargin().Bottom;
        }
    }
}