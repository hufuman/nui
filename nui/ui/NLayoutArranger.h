#pragma once


#include "NFrame.h"

namespace nui
{
    namespace Ui
    {
        class NLayoutArrangerParam
        {
        public:
            Base::NSize maxSize_;
            Base::NSize size_;
            Base::NSize frameSize_;
            INT data_;
        };

        class NUI_CLASS NLayoutArranger : public Base::NBaseObj
        {
        public:
            virtual void RelayoutChild(NFrameBase* child, NLayoutArrangerParam& param) const = 0;
            virtual bool IfCouldLayout(const NFrameBase* const &child) const = 0;
        };
    }
}
