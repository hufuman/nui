#include "stdafx.h"
#include "../NFrameBase.h"

#include "../NRenderClip.h"
#include "UiUtil.h"

namespace nui
{
    namespace Ui
    {
        bool NFrameBase::IsVertLayout(UINT layout)
        {
            return (layout & NFrameBase::LayoutTop)
                || (layout & NFrameBase::LayoutBottom)
                || (layout & NFrameBase::LayoutVCenter)
                || (layout & NFrameBase::LayoutVFill);
        }

        bool NFrameBase::IsHorzLayout(UINT layout)
        {
            return (layout & NFrameBase::LayoutLeft)
                || (layout & NFrameBase::LayoutRight)
                || (layout & NFrameBase::LayoutHCenter)
                || (layout & NFrameBase::LayoutHFill);
        }

        NFrameBase::NFrameBase()
        {
            parentFrame_ = NULL;
            topMostCount_ = 0;
            bottomMostCount_ = 0;
            frameFlags_ = FlagVisible | FlagAutoSize | FlagLayoutable | FlagValid;
            frameStatus_ = StatusNormal;
            layout_ = LayoutNone;
            frameData_ = 0;
        }

        NFrameBase::~NFrameBase()
        {
            NAssertError(Util::Misc::IsFlagChecked(frameFlags_, FlagValid), _T("NFrameBase not valid already"));
            frameFlags_ = 0;
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

            FrameList::const_iterator ite = GetChildHelper(child, zorder);
            for(int i=topMostCount_; i>0; --i)
            {
                -- ite;
            }
            childs_.insert(ite, child);
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

        NFrameBase* NFrameBase::GetChildByPointAndFlag(const Base::NPoint& point, DWORD flags, bool pointOffsetToParent)
        {
            if(!IsVisible()  || !IsEnabled())
                return NULL;

            Base::NPoint pt(point);
            if(!pointOffsetToParent && parentFrame_)
            {
                const Base::NRect& rootRect = parentFrame_->GetRootRect();
                pt.X -= rootRect.Left;
                pt.Y -= rootRect.Top;
            }

            if(!frameRect_.Contains(pt))
                return NULL;

            pt.X -= frameRect_.Left;
            pt.Y -= frameRect_.Top;

            NFrameBase* result = NULL;
            FrameList::const_reverse_iterator ite = childs_.rbegin();
            while(result == NULL && ite != childs_.rend())
            {
                NFrameBase* const& child = *ite;
                result = child->GetChildByPointAndFlag(pt, flags, true);
                ++ ite;
            }

            if(result == NULL && (frameFlags_ & flags))
                result = this;
            return result;
        }

        size_t NFrameBase::GetChildCount() const
        {
            return childs_.size();
        }

        NFrameBase* NFrameBase::GetParent() const
        {
            return parentFrame_;
        }

        bool NFrameBase::SetVisible(bool visible)
        {
            if(visible == IsVisible())
                return false;

            ForceInvalidate();
            Util::Misc::CheckFlag(frameFlags_, NFrameBase::FlagVisible, visible);

            UiUtil::SyncNativeWindowByVisible(this);
            return true;
        }

        bool NFrameBase::IsVisible() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrameBase::FlagVisible);
        }

        bool NFrameBase::SetEnabled(bool enabled)
        {
            if(enabled == IsEnabled())
                return false;

            ForceInvalidate();
            Util::Misc::CheckFlag(frameStatus_, NFrameBase::StatusDisabled, !enabled);

            UiUtil::SyncNativeWindowByEnable(this);
            return true;
        }

        bool NFrameBase::IsEnabled() const
        {
            return !Util::Misc::IsFlagChecked(frameStatus_, NFrameBase::StatusDisabled);
        }

        bool NFrameBase::SetCheck(bool checked)
        {
            if(checked == IsChecked())
                return false;
            ForceInvalidate();
            Util::Misc::CheckFlag(frameStatus_, NFrameBase::StatusChecked, checked);
            return true;
        }

        bool NFrameBase::IsChecked() const
        {
            return Util::Misc::IsFlagChecked(frameStatus_, NFrameBase::StatusChecked);
        }

        bool NFrameBase::IsInStatus(Status status) const
        {
            return (frameStatus_ & status) == static_cast<DWORD>(status);
        }

        void NFrameBase::SetId(const Base::NString& id)
        {
            frameId_ = id;
        }

        const Base::NString& NFrameBase::GetId() const
        {
            return frameId_;
        }

        NFrame* NFrameBase::SetData(DWORD data)
        {
            frameData_ = data;
            return dynamic_cast<NFrame*>(this);
        }

        DWORD NFrameBase::GetData() const
        {
            return frameData_;
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

        Base::NRect NFrameBase::GetScreenRect() const
        {
            if(window_ == NULL)
                return GetRootRect();
            Base::NRect windowRect;
            if(!window_->GetRect(windowRect))
                return GetRootRect();
            Base::NRect rootRect = GetRootRect();
            rootRect.Offset(windowRect.Left, windowRect.Top);
            return rootRect;
        }

        bool NFrameBase::SetPos(int left, int top)
        {
            return SetPosImpl(left, top, false);
        }

        bool NFrameBase::SetSize(int width, int height)
        {
            return SetSizeImpl(width, height, false);
        }

        bool NFrameBase::SetMinSize(int minWidth, int minHeight)
        {
            if(minSize_.Width == minWidth && minSize_.Height == minHeight)
                return false;
            minSize_.Width = minWidth;
            minSize_.Height = minHeight;
            if(IsAutoSize())
            {
                return AutoSize();
            }
            else if(frameRect_.Width() < minSize_.Width || frameRect_.Height() < minSize_.Height)
            {
                int width = frameRect_.Width() < minSize_.Width ? minSize_.Width : frameRect_.Width();
                int height = frameRect_.Height() < minSize_.Height ? minSize_.Height : frameRect_.Height();
                return SetSizeImpl(width, height, true);
            }
            return false;
        }

        bool NFrameBase::SetMaxSize(int maxWidth, int maxHeight)
        {
            if(maxSize_.Width == maxWidth && maxSize_.Height == maxHeight)
                return false;
            maxSize_.Width = maxWidth;
            maxSize_.Height = maxHeight;
            if(IsAutoSize())
            {
                return AutoSize();
            }
            else if(frameRect_.Width() > maxSize_.Width || frameRect_.Height() > maxSize_.Height)
            {
                int width = frameRect_.Width() > maxSize_.Width ? maxSize_.Width : frameRect_.Width();
                int height = frameRect_.Height() > maxSize_.Height ? maxSize_.Height : frameRect_.Height();
                return SetSizeImpl(width, height, true);
            }
            return false;
        }

        bool NFrameBase::AutoSize()
        {
            if(layout_ != LayoutNone)
            {
                ReLayout();
                return true;
            }
            if(!IsAutoSize())
                return false;
            Base::NSize autoSize = GetAutoSize();
            return SetSizeImpl(autoSize.Width, autoSize.Height, true);
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
            return minSize_;
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

        void NFrameBase::SetPadding(int left, int top, int right, int bottom)
        {
            if(left == padding_.Left
                && right == padding_.Right
                && top == padding_.Top
                && bottom == padding_.Bottom)
            {
                return;
            }
            padding_.SetRect(left, top, right, bottom);
            AutoSize();
        }

        const Base::NRect& NFrameBase::GetPadding() const
        {
            return padding_;
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

        UINT NFrameBase::GetLayout() const
        {
            return layout_;
        }

        void NFrameBase::ReLayout()
        {
            if(layout_ == LayoutNone || !IsLayoutable())
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

            Base::NRect rcNew(frameRect_.Left, frameRect_.Top, frameRect_.Left + size.Width, frameRect_.Top + size.Height);

            if(layout_ & LayoutHFill)
            {
                rcNew.Left = margin_.Left;
                rcNew.Right = width - margin_.Right;
            }
            else if(layout_ & LayoutHCenter)
            {
                size.Width = (minSize_.Width <= 0 || size.Width > minSize_.Width) ? size.Width : minSize_.Width;
                size.Width = (maxSize_.Width <= 0 || size.Width < maxSize_.Width) ? size.Width : maxSize_.Width;
                rcNew.Left = (width - margin_.Width() - size.Width) / 2;
                rcNew.Right = rcNew.Left + size.Width;
            }
            else if(layout_ & LayoutLeft)
            {
                rcNew.Left = margin_.Left;
                rcNew.Right = rcNew.Left + size.Width;
            }
            else if(layout_ & LayoutRight)
            {
                rcNew.Right = width - margin_.Right;
                rcNew.Left = rcNew.Right - size.Width;
            }

            if(layout_ & LayoutVFill)
            {
                rcNew.Top = margin_.Top;
                rcNew.Bottom = height - margin_.Bottom;
            }
            else if(layout_ & LayoutVCenter)
            {
                size.Height = (minSize_.Height <= 0 || size.Height > minSize_.Height) ? size.Height : minSize_.Height;
                size.Height = (maxSize_.Height <= 0 || size.Height < maxSize_.Height) ? size.Height : maxSize_.Height;
                rcNew.Top = (height - margin_.Height() - size.Height) / 2;
                rcNew.Bottom = rcNew.Top + size.Height;
            }
            else if(layout_ & LayoutTop)
            {
                rcNew.Top = margin_.Top;
                rcNew.Bottom = rcNew.Top + size.Height;
            }
            else if(layout_ & LayoutBottom)
            {
                rcNew.Bottom = height - margin_.Bottom;
                rcNew.Top = rcNew.Bottom - size.Height;
            }

            SetPosImpl(rcNew.Left, rcNew.Top, true);
            SetSizeImpl(rcNew.Width(), rcNew.Height(), true);
        }

        void NFrameBase::SetLayoutable(bool layoutable)
        {
            if(layoutable == IsLayoutable())
                return;
            Util::Misc::CheckFlag(frameFlags_, NFrameBase::FlagLayoutable, layoutable);
            if(layoutable)
                AutoSize();
        }

        bool NFrameBase::IsLayoutable() const
        {
            return Util::Misc::IsFlagChecked(frameFlags_, NFrameBase::FlagLayoutable);
        }

        void NFrameBase::Invalidate() const
        {
            if(!window_ || !IsVisible())
                return;
            ForceInvalidate();
        }

        void NFrameBase::ForceInvalidate() const
        {
            if(!window_)
                return;
            Base::NRect rootRect = GetRootRect();
            window_->InvalidateRect(rootRect);
        }

        void NFrameBase::Draw(NRender* render, Base::NPoint& ptOffset, HRGN clipRgn)
        {
            if(!IsVisible())
                return;

            Base::NRect rect(frameRect_);
            rect.Offset(ptOffset.X, ptOffset.Y);

            if(!::RectInRegion(clipRgn, rect))
                return;

            NRenderClip clip(render, clipRgn, rect);
            if(clip.IsEmpty())
                return;

            PreDrawEvent.Invoke(this, NULL);

            DrawBkg(render, rect);

            rect.Left += padding_.Left;
            rect.Top += padding_.Top;
            rect.Right -= padding_.Right;
            rect.Bottom -= padding_.Bottom;

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
            AutoSize();
        }

        void NFrameBase::OnMouseDown(short x, short y)
        {
            UNREFERENCED_PARAMETER(x);
            UNREFERENCED_PARAMETER(y);
            frameStatus_ |= StatusPressed;
            frameStatus_ |= StatusHover;
            Invalidate();
        }

        void NFrameBase::OnMouseUp()
        {
            frameStatus_ = frameStatus_ & (~StatusPressed);
            Invalidate();
        }

        void NFrameBase::OnMouseMove(short x, short y)
        {
            UNREFERENCED_PARAMETER(x);
            UNREFERENCED_PARAMETER(y);
        }

        void NFrameBase::OnMouseHover()
        {
            frameStatus_ |= StatusHover;
            if(NUiBus::Instance().GetCaptureFrame() == this)
                frameStatus_ |= StatusPressed;
            Invalidate();
        }

        void NFrameBase::OnMouseLeave()
        {
            frameStatus_ = frameStatus_ & (~StatusHover);
            frameStatus_ = frameStatus_ & (~StatusPressed);
            Invalidate();
        }

        bool NFrameBase::OnKeyDown(TCHAR key)
        {
            KeyEventData eventData;
            eventData.key = key;
            return !KeyDownEvent.Invoke(this, &eventData);
        }

        bool NFrameBase::OnKeyUp(TCHAR key)
        {
            KeyEventData eventData;
            eventData.key = key;
            return !KeyUpEvent.Invoke(this, &eventData);
        }

        bool NFrameBase::CanHover() const
        {
            return !!(FlagCanHover & frameFlags_);
        }

        void NFrameBase::OnSizeChanged(int width, int height)
        {
            UNREFERENCED_PARAMETER(width);
            UNREFERENCED_PARAMETER(height);
        }

        void NFrameBase::OnPosChanged(int left, int top)
        {
            UNREFERENCED_PARAMETER(left);
            UNREFERENCED_PARAMETER(top);
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

            bool childValid = true;
            if(newParent == NULL)
            {
                childValid = child->Release() > 0;
            }
            else
            {
                child->parentFrame_ = newParent;
                child->AddRef();
            }
            if(childValid)
            {
                child->OnParentChanged();
                if(child->parentFrame_)
                    child->parentFrame_->Invalidate();
            }
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
            if(!IsEnabled())
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
            if(!force && (layout_ == LayoutLeft || layout_ == LayoutRight || layout_ == LayoutHFill || layout_ == LayoutHCenter))
                left = frameRect_.Left;
            if(!force && (layout_ == LayoutTop || layout_ == LayoutBottom || layout_ == LayoutVFill || layout_ == LayoutVCenter))
                top = frameRect_.Top;
            if(frameRect_.Left == left && frameRect_.Top == top)
                return false;
            Invalidate();
            frameRect_.SetPos(left, top);
            OnPosChanged(left, top);
            Invalidate();
            return true;
        }

        bool NFrameBase::SetSizeImpl(int width, int height, bool force)
        {
            if(!force && (IsAutoSize() || (layout_ >= LayoutSizeStart && parentFrame_ != NULL)))
                return false;

            int frameWidth = (minSize_.Width < 0) ? width : (width >= minSize_.Width ? width : minSize_.Width);
            int frameHeight = (minSize_.Height < 0) ? height : (height >= minSize_.Height ? height : minSize_.Height);
            frameWidth = (maxSize_.Width > 0 && frameWidth > maxSize_.Width) ? maxSize_.Width : frameWidth;
            frameHeight = (maxSize_.Height > 0 && frameHeight > maxSize_.Height) ? maxSize_.Height : frameHeight;
            if(frameRect_.Width() == frameWidth && frameRect_.Height() == frameHeight)
                return false;

            Invalidate();
            frameRect_.SetSize(frameWidth, frameHeight);
            Invalidate();

            OnSizeChanged(frameWidth, frameHeight);

            SizeEventData eventData;
            eventData.width = width;
            eventData.height = height;
            SizeEvent.Invoke(this, &eventData);

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
