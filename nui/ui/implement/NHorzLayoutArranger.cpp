#include "stdafx.h"
#include "NHorzLayoutArranger.h"

#undef min
#undef max

namespace nui
{
    namespace Ui
    {

        bool NHorzLayoutArranger::IfCouldLayout(const NFrameBase* const &child) const
        {
            UINT flags = child->GetLayout();
            return ((flags & NFrameBase::LayoutLeft) != NFrameBase::LayoutLeft)
                || ((flags & NFrameBase::LayoutRight) != NFrameBase::LayoutRight)
                || ((flags & NFrameBase::LayoutHCenter) != NFrameBase::LayoutHCenter)
                || ((flags & NFrameBase::LayoutHFill) != NFrameBase::LayoutHFill);
        }

        void NHorzLayoutArranger::RelayoutChild(NFrameBase* child, Base::NSize& size) const
        {
            if(!IfCouldLayout(child))
                return;

            size.Width += child->GetMargin().Left;
            child->SetPos(size.Width, child->GetRect().Top);
            size.Height = std::max(child->GetRect().Height(), size.Height);
            size.Width += child->GetRect().Width() + child->GetMargin().Right;
        }
    }
}