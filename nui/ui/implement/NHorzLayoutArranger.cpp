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
            if(!child->IsVisible())
                return false;

            UINT flags = child->GetLayout();
            return !NFrameBase::IsHorzLayout(flags);
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