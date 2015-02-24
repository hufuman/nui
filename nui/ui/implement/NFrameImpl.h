#pragma once


#include "../NFrame.h"
#include "../../data/NArrayT.h"

namespace nui
{
    namespace Ui
    {
        class NFrameImpl : public nui::Ui::NFrame
        {
            typedef std::list<NFrameImpl*> FrameList;
        public:
            NFrameImpl();
            ~NFrameImpl();

            virtual bool AddChild(NFrame* child);
            virtual bool RemoveChild(NFrame* child);
            virtual void RemoveAllChilds();
            virtual size_t SetChildZOrder(NFrame* child, size_t zorder);
            virtual size_t GetChildZOrder(NFrame* child) const;
            virtual void SetChildTopmost(NFrame* child);
            virtual void SetChildBottommost(NFrame* child);
            virtual bool EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const;

            virtual NFrame* GetParent() const;

        private:
            static void SetParentHelper(NFrameImpl* childImpl, NFrame* newParent);
            FrameList::const_iterator GetChildHelper(NFrameImpl* childImpl, size_t& zorder) const;

        private:
            size_t topMostCount_;
            size_t bottomMostCount_;
            NFrame* parentFrame_;
            FrameList childs_;
        };
    }
}
