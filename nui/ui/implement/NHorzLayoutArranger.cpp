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

        void NHorzLayoutArranger::RelayoutChild(NFrameBase* child, NLayoutArrangerParam& param) const
        {
            if(!IfCouldLayout(child))
                return;

            param.maxSize_.Width += child->GetMargin().Left;
            child->SetPos(param.maxSize_.Width, child->GetRect().Top);
            param.maxSize_.Height = std::max(child->GetRect().Height(), param.maxSize_.Height);
            param.maxSize_.Width += child->GetRect().Width() + child->GetMargin().Right;
        }
    }
}