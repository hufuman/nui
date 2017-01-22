#include "stdafx.h"
#include "../NEvent.h"


namespace nui
{
    namespace Ui
    {
        NEvent::NEvent()
        {
            headNode_ = NULL;
            tailNode_ = NULL;
            embededLevel_ = 0;
        }

        NEvent::~NEvent()
        {
            Clear();
        }

        bool NEvent::AddHandler(NEventHandler handler)
        {
            NEventNode* node = NNew(NEventNode);
            node->handler = handler;
            node->valid = true;
            node->next = NULL;
            if(headNode_ == NULL)
            {
                headNode_ = tailNode_ = node;
            }
            else
            {
                tailNode_->next = node;
                tailNode_ = node;
            }
            return true;
        }

        bool NEvent::RemoveHandler(NEventHandler handler)
        {
            NEventNode* node = headNode_;
            if(headNode_ != NULL && headNode_->handler == handler)
            {
                headNode_->valid = false;
                if(embededLevel_ == 0)
                {
                    NDelete(headNode_);
                    if(node == tailNode_)
                        headNode_ = tailNode_ = NULL;
                    else
                        headNode_ = node->next;
                }
                node = NULL;
            }

            NEventNode* lastNode = headNode_;
            while(node != NULL)
            {
                if(node->valid && handler == node->handler)
                {
                    node->valid = false;
                    if(embededLevel_ == 0)
                    {
                        lastNode->next = node->next;
                        NDelete(node);
                        if(node == headNode_)
                        {
                            headNode_ = NULL;
                            tailNode_ = NULL;
                        }
                    }
                    return true;
                }
                lastNode = node;
                node = node->next;
            }
            return false;
        }

        void NEvent::Clear()
        {
            if(headNode_ == NULL)
                return;

            NEventNode* tmp;
            NEventNode* node = headNode_;
            while(node != NULL)
            {
                tmp = node->next;
                if(embededLevel_ == 0)
                    NDelete(node);
                else
                    node->valid = false;
                node = tmp;
            }
            if(embededLevel_ == 0)
            {
                headNode_ = tailNode_ = NULL;
            }
        }

        bool NEvent::Invoke(Base::NBaseObj* obj, NEventData* eventData)
        {
            ++ embededLevel_;
            NEventNode* node = headNode_;
            while(node != NULL)
            {
				if (node->valid && !node->handler(obj, eventData))
				{
					--embededLevel_;
					return false;
				}
                node = node->next;
            }
            -- embededLevel_;
            return true;
        }
    }
}
