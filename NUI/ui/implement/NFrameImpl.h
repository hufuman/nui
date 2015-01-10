#pragma once


#include "../NFrame.h"
#include "../../data/NArrayT.h"

namespace nui
{
    namespace UI
    {
        class NFrameImpl : public nui::UI::NFrame
        {
        public:
            NFrameImpl();
            ~NFrameImpl();

            virtual bool AddChild(NFrame* child);
            virtual bool RemoveChild(NFrame* child);
            virtual int GetChildZOrder(NFrame* child);
            virtual void SetChildTopmost(NFrame* child);
            virtual void SetChildBottommost(NFrame* child);
            virtual bool EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const;

            virtual void SetParent(NFrame* parent);
            virtual NFrame* GetParent() const;

        private:
            int topMostCount_;
            int bottomMostCount_;
            NFrame* parentContainer_;
            Data::NArrayT<NFrame*> childContainers_;
        };
    }
}
