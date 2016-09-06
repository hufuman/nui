#include "stdafx.h"
#include "../NLayout.h"

#include "../NRenderClip.h"

#include "NHorzLayoutArranger.h"
#include "NVertLayoutArranger.h"
#include "NTileLayoutArranger.h"

#undef min
#undef max

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NLayout);

        NLayout::NLayout() : innerFrame_(MemToolParam)
        {
            currentLayoutType_ = LayoutNone;
            SetLayoutType(LayoutVert);

            scrollAllowed_ = true;

            innerFrame_->Create(this, _NUI_INNER_FRAME_ID_, 0, _T(""));
            innerFrame_->SetAutoSize(false);

            GetHorzScroll();
            GetVertScroll();
        }

        NLayout::~NLayout()
        {
        }

        void NLayout::SetLayoutType(NLayoutType layoutType)
        {
            if(layoutType == currentLayoutType_)
                return;
            switch(layoutType)
            {
            case LayoutHorz:
                layoutArranger_ = NNew(NHorzLayoutArranger);
                break;
            case LayoutVert:
                layoutArranger_ = NNew(NVertLayoutArranger);
                break;
            case LayoutTile:
                layoutArranger_ = NNew(NTileLayoutArranger);
                break;
            default:
                layoutArranger_ = NULL;
            }
            currentLayoutType_ = layoutType;
            RelayoutChilds();
        }

        bool NLayout::AddChild(NFrameBase* child)
        {
            if(IsLayoutChild(child))
            {
                return __super::AddChild(child);
            }
            else
            {
                bool result = innerFrame_->AddChild(child);
                child->SizeEvent.AddHandler(MakeDelegate(this, &NLayout::OnChildSizeChanged));
                if(result)
                    RelayoutChilds();
                else
                    AutoSize();
                return result;
            }
        }

        bool NLayout::RemoveChild(NFrameBase* child)
        {
            if(IsLayoutChild(child))
            {
                return __super::AddChild(child);
            }
            else
            {
                child->SizeEvent.RemoveHandler(MakeDelegate(this, &NLayout::OnChildSizeChanged));
                bool result = innerFrame_->RemoveChild(child);
                if(result)
                    RelayoutChilds();
                else
                    AutoSize();
                return result;
            }
        }

        void NLayout::RemoveAllChilds()
        {
            innerFrame_->RemoveAllChilds();
            RelayoutChilds();
        }

        void NLayout::SetScrollAllowed(bool allow)
        {
            if(scrollAllowed_ != allow)
            {
                scrollAllowed_ = allow;
                RelayoutChilds();
            }
        }

        void NLayout::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:layout"));
#endif  // _NO_NUI_PARSER_
        }

        Base::NSize NLayout::GetAutoSize() const
        {
            return __super::GetAutoSize(autoSize_);
        }

        void NLayout::OnSizeChanged(int width, int height)
        {
            UNREFERENCED_PARAMETER(width);
            UNREFERENCED_PARAMETER(height);
            RelayoutChilds();
        }

        void NLayout::SetLayoutable(bool layoutable)
        {
            bool oldValue = IsLayoutable();
            __super::SetLayoutable(layoutable);
            if(!oldValue && layoutable)
            {
                RelayoutChilds();
            }
        }

        void NLayout::RelayoutChilds()
        {
            if(!IsLayoutable())
                return;

            if(innerFrame_ == NULL)
                return;

            if(delayRelayoutChilds_)
                return;

            Base::NInstPtr<NTimerSrv> timer(MemToolParam);
            delayRelayoutChilds_ = timer->startOnceTimer(10, MakeDelegate(this, &NLayout::RelayoutChildsImpl));
        }

        void NLayout::RelayoutChildsImpl()
        {
            delayRelayoutChilds_.Release();

            int vertScrollWidth = vertScroll_ ? vertScroll_->GetRect().Width() : 0;
            int horzScrollHeight = horzScroll_ ? horzScroll_->GetRect().Height() : 0;

            NLayoutArrangerParam param;
            param.data_ = 0;
            param.frameSize_ = frameRect_.GetSize();

            innerFrame_->EnumChilds(MakeDelegate(this, &NLayout::OnEnumChild), reinterpret_cast<LPARAM>(&param));

            if(param.maxSize_.Width == 0)
            {
                if(param.maxSize_.Height == 0)
                    param.maxSize_.Height = frameRect_.Height();

                if(param.maxSize_.Width == 0)
                    param.maxSize_.Width = param.maxSize_.Height > frameRect_.Height() ? frameRect_.Width() - vertScrollWidth : frameRect_.Width();
            }
            else if(param.maxSize_.Height == 0)
            {
                if(param.maxSize_.Width == 0)
                    param.maxSize_.Width = frameRect_.Width();

                if(param.maxSize_.Height == 0)
                    param.maxSize_.Height = param.maxSize_.Width > frameRect_.Width() ? frameRect_.Height() - horzScrollHeight : frameRect_.Height();
            }

            autoSize_.SetSize(param.maxSize_.Width, param.maxSize_.Height);
            innerFrame_->SetMinSize(param.maxSize_.Width, param.maxSize_.Height);
            innerFrame_->SetSize(param.maxSize_.Width, param.maxSize_.Height);

            const Base::NRect &rcLayout = GetRect();
            const Base::NRect &innerFrameRect = innerFrame_->GetRect();
            int innerFrameLeft = innerFrameRect.Left;
            int innerFrameTop = innerFrameRect.Top;
            if(innerFrameRect.Width() < rcLayout.Width())
                innerFrameLeft = 0;
            else if(innerFrameRect.Right + innerFrameRect.Left < rcLayout.Width())
                innerFrameLeft = rcLayout.Width() - innerFrameRect.Width();
            if(innerFrameRect.Height() < rcLayout.Height())
                innerFrameTop = 0;
            else if(innerFrameRect.Bottom + innerFrameRect.Top < rcLayout.Height())
                innerFrameTop = rcLayout.Height() - innerFrameRect.Height();

            if(!scrollAllowed_ || (rcLayout.Width() >= param.maxSize_.Width && rcLayout.Height() >= param.maxSize_.Height))
            {
                // both hide
                if(horzScroll_)
                    horzScroll_->SetVisible(false);
                if(vertScroll_)
                    vertScroll_->SetVisible(false);
                innerFrameLeft = 0;
                innerFrameTop = 0;
            }
            else if(param.maxSize_.Width + GetVertScroll()->GetRect().Width() <= frameRect_.Width() && param.maxSize_.Height > frameRect_.Height())
            {
                // horz hide, vert show
                if(horzScroll_)
                    horzScroll_->SetVisible(false);

                NScroll* vertScroll = GetVertScroll();
                vertScroll->SetScrollPage(rcLayout.Height());
                vertScroll->SetScrollRange(param.maxSize_.Height - rcLayout.Height());
                vertScroll->SetVisible(true);

                innerFrameLeft = 0;
                innerFrameTop = - vertScroll_->GetScrollPos();
            }
            else if(param.maxSize_.Height + GetHorzScroll()->GetRect().Height() <= frameRect_.Height() && param.maxSize_.Width > frameRect_.Width())
            {
                // vert hide, horz show
                if(vertScroll_)
                    vertScroll_->SetVisible(false);
                NScroll* horzScroll = GetHorzScroll();
                horzScroll->SetMargin(0, 0, 0, 0);
                horzScroll->SetScrollPage(rcLayout.Width());
                horzScroll->SetScrollRange(param.maxSize_.Width - rcLayout.Width());
                horzScroll->SetVisible(true);

                innerFrameTop = 0;
                innerFrameLeft = - horzScroll_->GetScrollPos();
            }
            else
            {
                // both show
                NScroll* horzScroll = GetHorzScroll();
                NScroll* vertScroll = GetVertScroll();
                horzScroll->SetMargin(0, 0, vertScroll->GetRect().Width(), 0);
                int horzPage = rcLayout.Width() - vertScroll->GetRect().Width();
                horzScroll->SetScrollPage(horzPage);
                horzScroll->SetScrollRange(param.maxSize_.Width - horzPage);
                horzScroll->SetVisible(true);

                int vertPage = rcLayout.Height() - horzScroll->GetRect().Height();
                vertScroll->SetScrollPage(vertPage);
                vertScroll->SetScrollRange(param.maxSize_.Height - vertPage);
                vertScroll->SetVisible(true);

                innerFrameLeft = - horzScroll_->GetScrollPos();
                innerFrameTop = - vertScroll_->GetScrollPos();
            }

            int width = frameRect_.Width() - vertScrollWidth;
            int height = frameRect_.Height() - (horzScroll_ && horzScroll_->IsVisible() ? horzScroll_->GetRect().Height() : 0);
            innerFrame_->SetMinSize(width, height);
            innerFrame_->SetPos(innerFrameLeft, innerFrameTop);

            AutoSize();
        }

        bool NLayout::OnEnumChild(NFrameBase* child, LPARAM lParam)
        {
            NLayoutArrangerParam* param = reinterpret_cast<NLayoutArrangerParam*>(lParam);

            layoutArranger_->RelayoutChild(child, *param);
            return true;
        }

        bool NLayout::OnHorzScrolled(Base::NBaseObj*, NEventData* eventData)
        {
            NScroll::ScrollEventData* data = static_cast<NScroll::ScrollEventData*>(eventData);
            innerFrame_->SetPos(-data->scrollPos, innerFrame_->GetRect().Top);
            return true;
        }

        bool NLayout::OnVertScrolled(Base::NBaseObj*, NEventData* eventData)
        {
            NScroll::ScrollEventData* data = static_cast<NScroll::ScrollEventData*>(eventData);
            innerFrame_->SetPos(innerFrame_->GetRect().Left, -data->scrollPos);
            return true;
        }

        bool NLayout::OnChildSizeChanged(Base::NBaseObj*, NEventData*)
        {
            RelayoutChilds();
            return true;
        }

        NScroll* NLayout::GetHorzScroll()
        {
            if(!scrollAllowed_)
                return NULL;

            if(horzScroll_)
                return horzScroll_;
            horzScroll_.Create(MemToolParam);
            horzScroll_->SetVisible(false);
            horzScroll_->Create(this, _NUI_HORZ_SCROLL_ID_, NFrame::LayoutHFill | NFrame::LayoutBottom, NULL);
            horzScroll_->SetScrollType(true);
            horzScroll_->ScrollEvent.AddHandler(MakeDelegate(this, &NLayout::OnHorzScrolled));
            return horzScroll_;
        }

        NScroll* NLayout::GetVertScroll()
        {
            if(!scrollAllowed_)
                return NULL;

            if(vertScroll_)
                return vertScroll_;
            vertScroll_.Create(MemToolParam);
            vertScroll_->SetVisible(false);
            vertScroll_->Create(this, _NUI_VERT_SCROLL_ID_, NFrame::LayoutVFill | NFrame::LayoutRight, NULL);
            vertScroll_->SetScrollType(false);
            vertScroll_->ScrollEvent.AddHandler(MakeDelegate(this, &NLayout::OnVertScrolled));
            return vertScroll_;
        }

        bool NLayout::IsLayoutChild(NFrameBase* child) const
        {
            return child == (NFrameBase*)horzScroll_
                || child == (NFrameBase*)vertScroll_
                || child == (NFrameBase*)innerFrame_;
        }
    }
}
