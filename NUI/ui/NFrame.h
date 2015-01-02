#pragma once


#include "../base/BaseObj.h"

namespace NUI
{
    namespace UI
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
        class NFrame;
        typedef FastDelegate2<NFrame*, LPARAM, bool> UiContainerEnumCallback;
        END_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NFrame : public NUI::Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("NFrame"))
        public:
            virtual bool AddChild(NFrame* child) = 0;
            virtual bool RemoveChild(NFrame* child) = 0;
            virtual int GetChildZOrder(NFrame* child) = 0;
            virtual void SetChildTopmost(NFrame* child) = 0;
            virtual void SetChildBottommost(NFrame* child) = 0;
            virtual bool EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const = 0;

            virtual void SetParent(NFrame* parent) = 0;
            virtual NFrame* GetParent() const = 0;
        };
    }
}
