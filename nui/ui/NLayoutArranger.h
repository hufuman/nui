#pragma once


#include "NFrame.h"

namespace nui
{
    namespace Ui
    {
        class NUI_CLASS NLayoutArranger : public Base::NBaseObj
        {
        public:
            virtual void RelayoutChild(NFrameBase* child, Base::NSize& size) const = 0;
            virtual bool IfCouldLayout(const NFrameBase* const &child) const = 0;
        };
    }
}
