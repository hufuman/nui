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
            if(!child->IsVisible())
                return false;

            UINT flags = child->GetLayout();
            return !NFrameBase::IsVertLayout(flags);
        }

        void NVertLayoutArranger::RelayoutChild(NFrameBase* child, NLayoutArrangerParam& param) const
        {
            if(!IfCouldLayout(child))
                return;

            UINT layout = child->GetLayout();

            const Base::NRect& margin = child->GetMargin();
            const Base::NRect &rect = child->GetRect();

            param.maxSize_.Height += margin.Top;
            child->SetPos(margin.Left, param.maxSize_.Height);

            param.maxSize_.Height += rect.Height() + margin.Bottom;

            int childWidth = (layout & NFrameBase::LayoutHFill) ? 0 : rect.Width();
            param.maxSize_.Width = std::max(childWidth + margin.Left + margin.Right, param.maxSize_.Width);
        }
    }
}