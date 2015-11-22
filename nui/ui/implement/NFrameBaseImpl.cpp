#include "stdafx.h"
#include "../NFrameBase.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        NFrameBase::NFrameBase()
        {
            parentFrame_ = NULL;
            topMostCount_ = 0;
            bottomMostCount_ = 0;
            frameFlags_ = FlagVisible | FlagValid | FlagEnabled;
            frameStatus_ = StatusNormal;
            layout_ = LayoutNone;
        }

        NFrameBase::~NFrameBase()
        {
            parentFrame_ = NULL;
            RemoveAllChilds();
        }

        bool NFrameBase::AddChild(NFrameBase* child)
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

        bool NFrameBase::RemoveChild(NFrameBase* child)
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

        void NFrameBase::RemoveAllChilds()
        {
            FrameList::iterator ite = childs_.begin();
            while(ite != childs_.end())
            {
                NFrameBase*& child = *ite;
                child->parentFrame_ = NULL;
                child->OnParentChanged();
                child->Release();
                ++ ite;
            }
            childs_.clear();
            topMostCount_ = 0;
            bottomMostCount_ = 0;
        }

        size_t NFrameBase::SetChildZOrder(NFrameBase* child, size_t zorder)
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

        size_t NFrameBase::GetChildZOrder(NFrameBase* child) const
        {
            NAssertError(child != NULL, _T("wrong type"));
            if(child == NULL)
                return static_cast<size_t>(-1);

            size_t zorder;
            FrameList::const_iterator ite = GetChildHelper(child, zorder);
            NAssertError(ite != childs_.end(), _T("child not exists"));
            return zorder;
        }

        void NFrameBase::SetChildTopmost(NFrameBase* child)
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

        void NFrameBase::SetChildBottommost(NFrameBase* child)
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

        bool NFrameBase::EnumChilds(UiContainerEnumCallback callback, LPARAM lParam) const
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

        NFrameBase* NFrameBase::GetChildById(const Base::NString& id, bool recursive)
        {
            if(frameId_ == id)
                return this;

            FrameList::const_iterator ite = childs_.begin();
            while(ite != childs_.end())
            {
                NFrameBase* child = *ite;
                if(recursive)
                {
                    NFrameBase* result = child->GetChildById(id, recursive);
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

        NFrameBase* NFrameBase::GetChildByPointAndFlag(const Base::NPoint& point, DWORD flags)
        {
            if(!IsVisible()  || !IsEnabled() || !IsValid())
                return NULL;

            Base::NPoint pt(point);
            if(!frameRect_.Contains(pt))
                return NULL;

            NFrameBase* result = NULL;
            FrameList::const_iterator ite = childs_.begin();
            while(result == NULL && ite != childs_.end())
            {
                NFrameBase* const& child = *ite;
                result = child->GetChildByPointAndFlag(pt, flags);
                ++ ite;
            }
            if(result == NULL && (frameFlags_ & flags))
                result = this;
            return result;
        }

        NFrameBase* NFrameBase::GetParent() const
        {
            return parentFrame_;
        }

        void NFrameBase::SetVisible(bool visible)
        {
            if(visible == IsVisible())
                return;
            Util::Misc::CheckFlag(frameFlags_, NFrameBase::FlagVisible, visible);
            Invalidate();
        }

        bool NFrameBase::IsVisible() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrameBase::FlagVisible);
        }

        void NFrameBase::SetEnabled(bool enabled)
        {
            if(enabled == IsEnabled())
                return;
            Util::Misc::CheckFlag(frameFlags_, NFrameBase::FlagEnabled, enabled);
            Invalidate();
        }

        bool NFrameBase::IsEnabled() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrameBase::FlagEnabled);
        }

        void NFrameBase::SetValid(bool valid)
        {
            if(valid == IsValid())
                return;
            Util::Misc::CheckFlag(frameFlags_, NFrameBase::FlagValid, valid);
            Invalidate();
        }

        bool NFrameBase::IsValid() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrameBase::FlagValid);
        }

        void NFrameBase::SetId(const Base::NString& id)
        {
            frameId_ = id;
        }

        const Base::NString& NFrameBase::GetId() const
        {
            return frameId_;
        }

        NWindow* NFrameBase::GetWindow()
        {
            return window_;
        }

        const Base::NRect& NFrameBase::GetRect() const
        {
            return frameRect_;
        }

        Base::NRect NFrameBase::GetRootRect() const
        {
            Base::NRect result = frameRect_;
            if(parentFrame_ != NULL)
            {
                Base::NRect parentRect = parentFrame_->GetRootRect();
                result.Offset(parentRect.Left, parentRect.Top);
            }
            return result;
        }

        void NFrameBase::SetPos(int left, int top)
        {
            SetPosImpl(left, top, false);
        }

        void NFrameBase::SetSize(int width, int height)
        {
            SetSizeImpl(width, height, false);
        }

        void NFrameBase::SetMinSize(int minWidth, int minHeight)
        {
            if(minSize_.Width == minWidth && minSize_.Height == minHeight)
                return;
            minSize_.Width = minWidth;
            minSize_.Height = minHeight;
            int width = frameRect_.Width() < minSize_.Width ? minSize_.Width : frameRect_.Width();
            int height = frameRect_.Height() < minSize_.Height ? minSize_.Height : frameRect_.Height();
            SetSizeImpl(width, height, true);
        }

        void NFrameBase::AutoSize()
        {
            if(!IsAutoSize() || layout_ != LayoutNone)
                return;
            Base::NSize autoSize = GetAutoSize();
            SetSizeImpl(autoSize.Width, autoSize.Height, true);
        }

        void NFrameBase::SetAutoSize(bool autosize)
        {
            if(autosize == IsAutoSize())
                return;
            Util::Misc::CheckFlag(frameFlags_, NFrameBase::FlagAutoSize, autosize);
            if(autosize)
                AutoSize();
        }

        Base::NSize NFrameBase::GetAutoSize() const
        {
            Base::NSize autoSize;
            return autoSize;
        }

        bool NFrameBase::IsAutoSize() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrame::FlagAutoSize);
        }

        void NFrameBase::SetMargin(int left, int top, int right, int bottom)
        {
            if(left == margin_.Left
                && right == margin_.Right
                && top == margin_.Top
                && bottom == margin_.Bottom)
            {
                return;
            }
            margin_.SetRect(left, top, right, bottom);
            ReLayout();
        }

        const Base::NRect& NFrameBase::GetMargin() const
        {
            return margin_;
        }

        void NFrameBase::SetLayout(UINT layout)
        {
            if(layout_ == layout)
                return;
            layout_ = layout;
            ReLayout();
        }

        void NFrameBase::ReLayout()
        {
            if(layout_ == LayoutNone)
                return;

            int width = 0;
            int height = 0;

            if(GetParent() != NULL)
            {
                Base::NRect rcParent = GetParent()->GetRect();
                width = rcParent.Width();
                height = rcParent.Height();
            }
            else if(window_ != NULL)
            {
                Base::NRect rcWnd;
                window_->GetRect(rcWnd);
                width = rcWnd.Width();
                height = rcWnd.Height();
            }
            else
            {
                return;
            }

            Base::NSize size = IsAutoSize() ? GetAutoSize() : frameRect_.GetSize();

            Base::NRect rcNew;

            if(layout_ & LayoutLeft)
            {
                rcNew.Left = margin_.Left;
                rcNew.Right = rcNew.Left + size.Width;
            }
            else if(layout_ & LayoutRight)
            {
                rcNew.Right = width - margin_.Right;
                rcNew.Left = rcNew.Right - size.Width;
            }
            else if(layout_ & LayoutHCenter)
            {
                rcNew.Left = (width - margin_.Width() - size.Width) / 2;
                rcNew.Right = rcNew.Left + size.Width;
            }
            else if(layout_ & LayoutHFill)
            {
                rcNew.Left = margin_.Left;
                rcNew.Right = width - margin_.Right;
            }

            if(layout_ & LayoutTop)
            {
                rcNew.Top = margin_.Top;
                rcNew.Bottom = rcNew.Top + size.Height;
            }
            else if(layout_ & LayoutBottom)
            {
                rcNew.Bottom = height - margin_.Bottom;
                rcNew.Top = rcNew.Bottom - size.Height;
            }
            else if(layout_ & LayoutVCenter)
            {
                rcNew.Top = (height - margin_.Height() - size.Height) / 2;
                rcNew.Bottom = rcNew.Top + size.Height;
            }
            else if(layout_ & LayoutVFill)
            {
                rcNew.Top = margin_.Top;
                rcNew.Bottom = height - margin_.Bottom;
            }

            SetPosImpl(rcNew.Left, rcNew.Top, true);
            SetSizeImpl(rcNew.Width(), rcNew.Height(), true);
        }

        void NFrameBase::Invalidate() const
        {
            if(!window_ || !IsVisible() || !IsValid())
                return;
            Base::NRect rootRect = GetRootRect();
            window_->InvalidateRect(rootRect);
        }

        void NFrameBase::Draw(NRender* render, Base::NPoint& ptOffset, HRGN clipRgn)
        {
            if(!IsVisible() || !IsValid())
                return;

            Base::NRect rect(frameRect_);
            rect.Offset(ptOffset.X, ptOffset.Y);

            if(!::RectInRegion(clipRgn, rect))
                return;

            NRenderClip clip(render, clipRgn, rect);

            DrawBkg(render, rect);
            DrawContent(render, rect);
            DrawFore(render, rect);

            DrawChilds(render, ptOffset, clipRgn);
        }

        void NFrameBase::OnParentChanged()
        {
            if(window_)
                window_ = NULL;
            if(parentFrame_)
                OnWindowChanged(parentFrame_->window_);
            else
                OnWindowChanged(NULL);
            ReLayout();
        }

        void NFrameBase::OnWindowChanged(NWindow* window)
        {
            if(window == window_)
                return;
            window_ = window;
            FrameList::iterator ite = childs_.begin();
            while(ite != childs_.end())
            {
                NFrameBase*& child = *ite;
                child->OnWindowChanged(window_);
                ++ ite;
            }
        }

        void NFrameBase::UpdateStatus(DWORD dwStatus, bool bAdd)
        {
            DWORD newStatus;
            if(bAdd)
                newStatus = frameStatus_ | dwStatus;
            else
                newStatus = frameStatus_ & (~dwStatus);
            if(frameStatus_ == newStatus)
                return;
            frameStatus_ = newStatus;
            Invalidate();
        }

        bool NFrameBase::CanHover() const
        {
            return !!(FlagCanHover & frameFlags_);
        }

        void NFrameBase::SetParentHelper(NFrameBase* child, NFrameBase* newParent)
        {
            if(newParent == child->parentFrame_)
                return;
            NAssertError(child != NULL, _T("wrong type"));

            if(child->parentFrame_ != NULL)
            {
                NFrameBase* parent = child->parentFrame_;
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

        NFrameBase::FrameList::const_iterator NFrameBase::GetChildHelper(NFrameBase* child, size_t& zorder) const
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

        void NFrameBase::GetDrawIndex(int& horzIndex, int& vertIndex) const
        {
            if(frameStatus_ & StatusDisabled)
                horzIndex = 3;
            else if(frameStatus_ & StatusPressed)
                horzIndex = 2;
            else if(frameStatus_ & StatusHover)
                horzIndex = 1;
            else
                horzIndex = 0;
            vertIndex = (frameStatus_ & StatusChecked) ? 1 : 0;
        }

        void NFrameBase::DrawBkg(NRender* render, const Base::NRect& rect) const
        {
            UNREFERENCED_PARAMETER(render);
            UNREFERENCED_PARAMETER(rect);
        }

        void NFrameBase::DrawFore(NRender* render, const Base::NRect& rect) const
        {
            UNREFERENCED_PARAMETER(render);
            UNREFERENCED_PARAMETER(rect);
        }

        void NFrameBase::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            UNREFERENCED_PARAMETER(render);
            UNREFERENCED_PARAMETER(rect);
        }

        void NFrameBase::DrawChilds(NRender* render, Base::NPoint& ptOffset, HRGN clipRgn)
        {
            ptOffset.Offset(frameRect_.Left, frameRect_.Top);
            FrameList::const_iterator ite = childs_.begin();
            for(; ite != childs_.end(); ++ ite)
            {
                NFrameBase* const & child = *ite;
                child->Draw(render, ptOffset, clipRgn);
            }
            ptOffset.Offset(- frameRect_.Left, - frameRect_.Top);
        }

        bool NFrameBase::SetPosImpl(int left, int top, bool force)
        {
            if(layout_ > 0 && layout_ <= LayoutPosEnd && !force)
                return false;
            if(frameRect_.Left == left && frameRect_.Top == top)
                return false;
            Invalidate();
            frameRect_.Left = left;
            frameRect_.Top = top;
            Invalidate();
            return true;
        }

        bool NFrameBase::SetSizeImpl(int width, int height, bool force)
        {
            if(!force && (IsAutoSize() || layout_ >= LayoutSizeStart))
                return false;

            int frameWidth = (minSize_.Width < 0) ? width : (width >= minSize_.Width ? width : minSize_.Width);
            int frameHeight = (minSize_.Height < 0) ? height : (height >= minSize_.Height ? height : minSize_.Height);
            if(frameRect_.Width() == width && frameRect_.Height() == height)
                return false;

            Invalidate();
            frameRect_.SetSize(frameWidth, frameHeight);
            Invalidate();

            // Relayout all children
            FrameList::const_iterator ite = childs_.begin();
            while(ite != childs_.end())
            {
                NFrameBase* const& child = *ite;
                child->ReLayout();
                ++ ite;
            }
            return true;
        }
    }
}
