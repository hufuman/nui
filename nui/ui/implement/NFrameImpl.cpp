#include "stdafx.h"
#include "NFrameImpl.h"


namespace nui
{
    namespace Ui
    {
        NFrameImpl::NFrameImpl()
        {
            parentContainer_ = NULL;
        }

        NFrameImpl::~NFrameImpl()
        {
            childContainers_.RemoveAllItems();
        }

        bool NFrameImpl::AddChild(NFrame* child)
        {
            if(!childContainers_.AddItem(child))
                return false;
            child->SetParent(this);
            child->AddRef();
            return true;
        }

        bool NFrameImpl::RemoveChild(NFrame* child)
        {
            if(!childContainers_.RemoveItem(child))
                return false;
            child->SetParent(NULL);
            child->Release();
            return true;
        }

        int NFrameImpl::GetChildZOrder(NFrame* child)
        {
            int pos = childContainers_.GetItemPos(child);
            return pos;
        }

        void NFrameImpl::SetChildTopmost(NFrame* child)
        {
            int pos = childContainers_.GetItemPos(child);
            if(pos == -1)
                return;
            childContainers_.ChangeItemPos(pos, 0);
            if(pos >= topMostCount_)
            {
                ++ topMostCount_;
            }
        }

        void NFrameImpl::SetChildBottommost(NFrame* child)
        {
            int pos = childContainers_.GetItemPos(child);
            if(pos == -1)
                return;
            childContainers_.ChangeItemPos(pos, childContainers_.Count() - 1);
            if(pos < childContainers_.Count() - bottomMostCount_)
            {
                ++ bottomMostCount_;
            }
        }

        bool NFrameImpl::EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const
        {
            int count = childContainers_.Count();
            for(int i=0; i<count; ++ i)
            {
                if(!callback(childContainers_[i], lParam))
                    return false;
            }
            return true;
        }

        void NFrameImpl::SetParent(NFrame* parent)
        {
            if(parentContainer_ != NULL)
                parentContainer_->Release();
            parentContainer_ = parent;
            if(parentContainer_ != NULL)
                parentContainer_->AddRef();
        }

        NFrame* NFrameImpl::GetParent() const
        {
            return parentContainer_;
        }

    }
}
