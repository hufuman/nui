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
            if(!child->IsVisible() || child->GetRect().Width() == 0 || child->GetRect().Height() == 0)
                return false;

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

            const Base::NRect& margin = child->GetMargin();
            param.maxSize_.Width += margin.Left;
            child->SetPos(param.maxSize_.Width, margin.Top);
            param.maxSize_.Width += child->GetRect().Width() + margin.Right;
            param.maxSize_.Height = std::max(child->GetRect().Height() + margin.Top + margin.Bottom, param.maxSize_.Height);
        }
    }
}