#include "stdafx.h"
#include "../NFrame.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NFrame);

        NFrame::NFrame()
        {
            parentFrame_ = NULL;
            topMostCount_ = 0;
            bottomMostCount_ = 0;
            frameFlags_ = FlagVisible | FlagValid | FlagEnabled;
            frameStatus_ = StatusNormal;
        }

        NFrame::~NFrame()
        {
            parentFrame_ = NULL;
            RemoveAllChilds();
        }

        void NFrame::SetClickCallback(ClickEventCallback callback)
        {
            clickCallback_ = callback;
        }

        bool NFrame::AddChild(NFrame* child)
        {
            NAssertError(child != NULL, _T("wrong type"));
            if(child == NULL)
                return FALSE;

            // check if exists
            size_t zorder;
            NAssertError(GetChildHelper(child, zorder) == childs_.end(), _T("child already exists"));
            if(GetChildHelper(child, zorder) != childs_.end())
                return true;

            childs_.push_back(child);
            SetParentHelper(child, this);
            return true;
        }

        bool NFrame::RemoveChild(NFrame* child)
        {
            NAssertError(child != NULL, _T("wrong type"));
            if(child == NULL)
                return FALSE;

            size_t zorder;
            FrameList::const_iterator ite = GetChildHelper(child, zorder);
            if(ite == childs_.end())
                return false;

            if(zorder < topMostCount_)
                -- topMostCount_;
            if(zorder >= childs_.size() - bottomMostCount_)
                -- bottomMostCount_;

            childs_.erase(ite);
            SetParentHelper(child, NULL);
            return true;
        }

        void NFrame::RemoveAllChilds()
        {
            FrameList::iterator ite = childs_.begin();
            while(ite != childs_.end())
            {
                NFrame*& child = *ite;
                child->parentFrame_ = NULL;
                child->OnParentChanged();
                child->Release();
                ++ ite;
            }
            childs_.clear();
            topMostCount_ = 0;
            bottomMostCount_ = 0;
        }

        size_t NFrame::SetChildZOrder(NFrame* child, size_t zorder)
        {
            NAssertError(child != NULL, _T("wrong type"));
            if(child == NULL)
                return static_cast<size_t>(-1);

            size_t tmpZorder;
            FrameList::const_iterator ite = GetChildHelper(child, tmpZorder);
            NAssertError(ite != childs_.end(), _T("child not exists"));
            if(ite == childs_.end())
                return static_cast<size_t>(-1);

            if(tmpZorder < topMostCount_ || tmpZorder >= childs_.size() - bottomMostCount_)
                return tmpZorder;

            childs_.erase(ite);
            zorder = (zorder > childs_.size()) ? childs_.size() : zorder;
            zorder = (zorder < 0) ? 0 : zorder;
            ite = childs_.begin();
            for(size_t i=0; i<zorder; ++ i)
                ++ ite;
            childs_.insert(ite, child);
            return zorder;
        }

        size_t NFrame::GetChildZOrder(NFrame* child) const
        {
            NAssertError(child != NULL, _T("wrong type"));
            if(child == NULL)
                return static_cast<size_t>(-1);

            size_t zorder;
            FrameList::const_iterator ite = GetChildHelper(child, zorder);
            NAssertError(ite != childs_.end(), _T("child not exists"));
            return zorder;
        }

        void NFrame::SetChildTopmost(NFrame* child)
        {
            NAssertError(child != NULL, _T("wrong type"));
            if(child == NULL)
                return;

            size_t zorder;
            FrameList::const_iterator ite = GetChildHelper(child, zorder);
            NAssertError((ite != childs_.end()), _T("child not exists"));
            if(ite == childs_.end())
                return;
            if(zorder < topMostCount_)
                return;
            if(zorder >= childs_.size() - bottomMostCount_)
                -- bottomMostCount_;
            // move
            childs_.erase(ite);
            ite = childs_.begin();
            for(size_t i=0; i<topMostCount_; ++ i)
            {
                ++ ite;
            }
            childs_.insert(ite, child);
            ++ topMostCount_;
        }

        void NFrame::SetChildBottommost(NFrame* child)
        {
            NAssertError(child != NULL, _T("wrong type"));
            if(child == NULL)
                return;

            size_t zorder;
            FrameList::const_iterator ite = GetChildHelper(child, zorder);
            NAssertError((ite != childs_.end()), _T("child not exists"));
            if(ite == childs_.end())
                return;
            if(zorder >= childs_.size() - bottomMostCount_)
                return;
            if(zorder < topMostCount_)
                -- topMostCount_;
            // move
            childs_.erase(ite);
            ite = childs_.begin();
            size_t offset = childs_.size() - bottomMostCount_;
            for(size_t i=0; i<offset; ++ i)
            {
                ++ ite;
            }
            childs_.insert(ite, child);
            ++ bottomMostCount_;
        }

        bool NFrame::EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const
        {
            FrameList::const_iterator ite = childs_.begin();
            while(ite != childs_.end())
            {
                if(!callback(*ite, lParam))
                    return false;
                ++ ite;
            }
            return true;
        }

        NFrame* NFrame::GetChildById(const Base::NString& id, bool recursive)
        {
            if(frameId_ == id)
                return this;

            FrameList::const_iterator ite = childs_.begin();
            while(ite != childs_.end())
            {
                NFrame* child = *ite;
                if(recursive)
                {
                    NFrame* result = child->GetChildById(id, recursive);
                    if(result != NULL)
                        return result;
                }
                else
                {
                    if(child->frameId_ == id)
                        return child;
                }
                ++ ite;
            }
            return NULL;
        }

        NFrame* NFrame::GetChildByPointAndFlag(const Base::NPoint& point, DWORD flags)
        {
            if(!IsVisible()  || !IsEnabled() || !IsValid())
                return NULL;

            Base::NPoint pt(point);
            if(!frameRect_.Contains(pt))
                return NULL;

            NFrame* result = NULL;
            FrameList::const_iterator ite = childs_.begin();
            while(result == NULL && ite != childs_.end())
            {
                NFrame* const& child = *ite;
                result = child->GetChildByPointAndFlag(pt, flags);
                ++ ite;
            }
            if(result == NULL && (frameFlags_ & flags))
                result = this;
            return result;
        }

        NFrame* NFrame::GetParent() const
        {
            return parentFrame_;
        }

        void NFrame::SetVisible(bool visible)
        {
            if(visible == IsVisible())
                return;
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagVisible, visible);
            Invalidate();
        }

        bool NFrame::IsVisible() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrame::FlagVisible);
        }

        void NFrame::SetEnabled(bool enabled)
        {
            if(enabled == IsEnabled())
                return;
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagEnabled, enabled);
            Invalidate();
        }

        bool NFrame::IsEnabled() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrame::FlagEnabled);
        }

        void NFrame::SetValid(bool valid)
        {
            if(valid == IsValid())
                return;
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagValid, valid);
            Invalidate();
        }

        bool NFrame::IsValid() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrame::FlagValid);
        }

        void NFrame::SetId(const Base::NString& id)
        {
            frameId_ = id;
        }

        Base::NString NFrame::GetId() const
        {
            return frameId_;
        }

        const Base::NRect& NFrame::GetRect() const
        {
            return frameRect_;
        }

        Base::NRect NFrame::GetRootRect() const
        {
            Base::NRect result = frameRect_;
            if(parentFrame_ != NULL)
            {
                Base::NRect parentRect = parentFrame_->GetRootRect();
                result.Offset(parentRect.Left, parentRect.Top);
            }
            return result;
        }

        void NFrame::SetPos(int left, int top)
        {
            if(frameRect_.Left == left && frameRect_.Top == top)
                return;
            frameRect_.Left = left;
            frameRect_.Top = top;
            Invalidate();
        }

        void NFrame::SetSize(int width, int height)
        {
            int frameWidth = (minSize_.Width < 0) ? width : (width >= minSize_.Width ? width : minSize_.Width);
            int frameHeight = (minSize_.Height < 0) ? height : (height >= minSize_.Height ? height : minSize_.Height);
            if(frameRect_.Width() == width && frameRect_.Height() == height)
                return;

            frameRect_.SetSize(frameWidth, frameHeight);
            Invalidate();
        }

        void NFrame::SetMinSize(int minWidth, int minHeight)
        {
            minSize_.Width = minWidth;
            minSize_.Height = minHeight;
        }

        void NFrame::Invalidate() const
        {
            if(!window_)
                return;
            Base::NRect rootRect = GetRootRect();
            window_->InvalidateRect(rootRect);
        }

        void NFrame::Draw(NRender* render, Base::NPoint& ptOffset, const Base::NRect& clipRect)
        {
            if(!IsVisible() || !IsValid())
                return;

            Base::NRect rect(frameRect_);
            rect.Offset(ptOffset.X, ptOffset.Y);

            if(!clipRect.Overlap(rect))
                return;

            NRenderClip clip(render, clipRect, rect);

            DrawBkg(render, rect);
            DrawContent(render, rect);
            DrawFore(render, rect);

            DrawChilds(render, ptOffset, clipRect);
        }

        void NFrame::OnParentChanged()
        {
            if(window_)
                window_ = NULL;
            if(parentFrame_)
                OnWindowChanged(parentFrame_->window_);
            else
                OnWindowChanged(NULL);
        }

        void NFrame::OnWindowChanged(NWindow* window)
        {
            if(window == window_)
                return;
            window_ = window;
            FrameList::iterator ite = childs_.begin();
            while(ite != childs_.end())
            {
                NFrame*& child = *ite;
                child->OnWindowChanged(window_);
                ++ ite;
            }
        }

        void NFrame::OnClicked(const nui::Base::NPoint& point)
        {
            if(clickCallback_)
                clickCallback_(this, point);
        }

        void NFrame::CancelHover()
        {
            if(!(frameStatus_ & StatusHover))
                return;

            frameStatus_ = frameFlags_ & (~StatusHover);
            Invalidate();
        }

        void NFrame::BeginHover()
        {
            if((frameStatus_ & StatusHover) || !CanHover())
                return;

            frameStatus_ = frameFlags_ | StatusHover;
            Invalidate();
        }

        bool NFrame::CanHover() const
        {
            return !!(FlagCanHover & frameFlags_);
        }

        void NFrame::SetParentHelper(NFrame* child, NFrame* newParent)
        {
            if(newParent == child->parentFrame_)
                return;
            NAssertError(child != NULL, _T("wrong type"));

            if(child->parentFrame_ != NULL)
            {
                NFrame* parent = child->parentFrame_;
                parent->RemoveChild(child);
                child->parentFrame_ = NULL;
                parent->Invalidate();
            }
            if(newParent == NULL)
            {
                child->Release();
            }
            else
            {
                child->parentFrame_ = newParent;
                child->AddRef();
            }
            child->OnParentChanged();
            if(child->parentFrame_)
                child->parentFrame_->Invalidate();
        }

        NFrame::FrameList::const_iterator NFrame::GetChildHelper(NFrame* child, size_t& zorder) const
        {
            zorder = 0;
            FrameList::const_iterator ite = childs_.begin();
            while(ite != childs_.end() && (*ite) != child)
            {
                ++ zorder;
                ++ ite;
            }
            if(ite == childs_.end())
            {
                zorder = static_cast<size_t>(-1);
                return childs_.end();
            }
            else
            {
                return ite;
            }
        }

        void NFrame::GetDrawIndex(int& horzIndex, int& vertIndex) const
        {
            horzIndex = (frameStatus_ == StatusDisabled) ? 3 : (frameStatus_ & 0x0F) / 2;
            vertIndex = (frameStatus_ & StatusChecked) ? 1 : 0;
        }

        void NFrame::DrawBkg(NRender* render, const Base::NRect& rect) const
        {
            UNREFERENCED_PARAMETER(render);
            UNREFERENCED_PARAMETER(rect);
        }

        void NFrame::DrawFore(NRender* render, const Base::NRect& rect) const
        {
            UNREFERENCED_PARAMETER(render);
            UNREFERENCED_PARAMETER(rect);
        }

        void NFrame::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            UNREFERENCED_PARAMETER(render);
            UNREFERENCED_PARAMETER(rect);
        }

        void NFrame::DrawChilds(NRender* render, Base::NPoint& ptOffset, const Base::NRect& clipRect)
        {
            ptOffset.Offset(frameRect_.Left, frameRect_.Top);
            FrameList::const_iterator ite = childs_.begin();
            for(; ite != childs_.end(); ++ ite)
            {
                NFrame* const & child = *ite;
                child->Draw(render, ptOffset, clipRect);
            }
            ptOffset.Offset(- frameRect_.Left, - frameRect_.Top);
        }
    }
}
