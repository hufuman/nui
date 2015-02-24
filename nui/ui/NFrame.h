#pragma once


#include "../base/BaseObj.h"

namespace nui
{
    namespace Ui
    {
        BEGIN_USE_UNEXPORT_TEMPLATE()
        class NFrame;
        typedef FastDelegate2<NFrame*, LPARAM, bool> UiContainerEnumCallback;
        END_USE_UNEXPORT_TEMPLATE()

        class NUI_CLASS NFrame : public nui::Base::NBaseObj
        {
            DECLARE_REFLECTION(TEXT("nui"), TEXT("frame"))
        public:
            virtual bool AddChild(NFrame* child) = 0;
            virtual bool RemoveChild(NFrame* child) = 0;
            virtual void RemoveAllChilds() = 0;
            virtual size_t SetChildZOrder(NFrame* child, size_t zorder) = 0;
            virtual size_t GetChildZOrder(NFrame* child) const = 0;
            virtual void SetChildTopmost(NFrame* child) = 0;
            virtual void SetChildBottommost(NFrame* child) = 0;
            virtual bool EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const = 0;

            virtual NFrame* GetParent() const = 0;
        };
    }
}
