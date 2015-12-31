#include "stdafx.h"
#include "../NList.h"

#include "../NRenderClip.h"

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NList);

        namespace
        {
            const TCHAR g_SubFrameIdPrefix[] = _T("_SubFrame_");
        }

        NList::NList()
        {
            columnId_ = 0;
            normalLineHeight_ = 80;
        }

        NList::~NList()
        {
        }

        int NList::AddColumn(int columnIndex, LPCTSTR text, int width)
        {
            return AddColumn(columnIndex, text, width, ListFlagNone);
        }

        int NList::AddColumn(int columnIndex, LPCTSTR text, int width, UINT flags)
        {
            UINT headerFlag = NHeader::HeaderFlagNone;
            if(flags & ListFlagNoSort)
                headerFlag |= NHeader::HeaderFlagNoSort;
            if(flags & ListFlagFixedWidth)
                headerFlag |= NHeader::HeaderFlagFixedWidth;
            int index = header_->AddItem(columnIndex, text, width, headerFlag);
            header_->SetItemData(index, ++ columnId_);
            return index;
        }

        void NList::RemoveColumn(int columnIndex)
        {
            header_->RemoveItem(columnIndex);
        }

        int NList::AddItem(int lineIndex)
        {
            Base::NInstPtr<NFrame> lineFrame(MemToolParam);
            lineFrame.Create(MemToolParam);
            Base::NRect rcLine(0, 0, GetLineFrameWidth(), normalLineHeight_);
            int lineWidth = GetLineFrameWidth();
            lineFrame->Create(layout_, NULL, rcLine, NULL);
            int newLineWidth = GetLineFrameWidth();
            if(lineWidth != newLineWidth)
                ResetLineFrameWidth();
            return static_cast<int>(layout_->GetChildById(_NUI_INNER_FRAME_ID_, false)->SetChildZOrder(lineFrame, lineIndex));
        }

        void NList::RemoveItem(int lineIndex)
        {
            NFrame* lineFrame = layout_->GetChildById(_NUI_INNER_FRAME_ID_, false)->GetChildAtIndex(static_cast<size_t>(lineIndex));
            if(lineFrame != NULL)
            {
                int lineWidth = GetLineFrameWidth();
                layout_->RemoveChild(lineFrame);
                int newLineWidth = GetLineFrameWidth();
                if(lineWidth != newLineWidth)
                    ResetLineFrameWidth();
            }
        }

        void NList::RemoveAllItems()
        {
            layout_->RemoveAllChilds();
            ResetLineFrameWidth();
        }

        NFrame* NList::GetLineFrame(int lineIndex)
        {
            NFrame* lineFrame = layout_->GetChildById(_NUI_INNER_FRAME_ID_, false)->GetChildAtIndex(static_cast<size_t>(lineIndex));
            if(lineFrame == NULL)
                return NULL;

            if(lineFrame->GetChildCount() > 0)
                return lineFrame;

            // refill sub frames
            Base::NRect rcSubFrame;
            int frameLeft = 0;
            int columnCount = header_->GetItemCount();
            Base::NString frameId;
            int frameHeight = lineFrame->GetRect().Height();
            for(int i=0; i<columnCount; ++ i)
            {
                Base::NInstPtr<NFrame> subFrame(MemToolParam);
                int width = header_->GetItemWidth(i);
                rcSubFrame.SetRect(frameLeft, 0, frameLeft + width, frameHeight);
                GetSubFrameId(i, frameId);
                subFrame->Create(lineFrame, frameId, rcSubFrame, NULL);
                frameLeft += width;
            }
            return lineFrame;
        }

        int NList::GetItemCount() const
        {
            return layout_->GetChildById(_NUI_INNER_FRAME_ID_, false)->GetChildCount();
        }

        NFrame* NList::GetSubFrame(int lineIndex, int columnIndex)
        {
            if(columnIndex >= header_->GetItemCount())
                return NULL;

            NFrame* lineFrame = GetLineFrame(lineIndex);
            if(lineFrame == NULL)
                return NULL;

            Base::NString frameId;
            GetSubFrameId(columnIndex, frameId);
            return lineFrame->GetChildById(frameId, false);
        }

        void NList::OnCreate()
        {
            header_.Create(MemToolParam);
            header_->Create(this, _NUI_HEADER_ID_, 0, NULL);
            header_->SetMinSize(GetRect().Width(), 0);

            header_->ItemWidthChangeEvent.AddHandler(MakeDelegate(this, &NList::OnHeaderItemWidthChanged));

            layout_.Create(MemToolParam);
            layout_->Create(this, _NUI_LAYOUT_ID_, NFrame::LayoutHFill | NFrame::LayoutVFill, NULL);
            layout_->SetMargin(0, header_->GetRect().Height(), 0, 0);
            layout_->GetChildById(_NUI_INNER_FRAME_ID_, false)->SetMinSize(header_->GetRect().Width(), -1);

            NScroll* horzScroll = dynamic_cast<NScroll*>(layout_->GetChildById(_NUI_HORZ_SCROLL_ID_, false));
            horzScroll->ScrollEvent.AddHandler(MakeDelegate(this, &NList::OnHorzScrollPosChanged));

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:list"));
#endif  // _NO_NUI_PARSER_
        }

        void NList::OnSizeChanged(int width, int height)
        {
            UNREFERENCED_PARAMETER(width);
            UNREFERENCED_PARAMETER(height);
            if(header_)
            {
                header_->SetMinSize(width, 0);
                ResetLineFrameWidth();
            }
        }

        void NList::GetSubFrameId(int columnIndex, Base::NString& subFrameId) const
        {
            subFrameId.Format(_T("%s%u"), g_SubFrameIdPrefix, header_->GetItemData(columnIndex));
        }

        bool NList::OnAdjustLinewWidths(NFrameBase* child, LPARAM lParam)
        {
            UNREFERENCED_PARAMETER(lParam);
            Base::NString frameId;
            int columnCount = header_->GetItemCount();

            int lineFrameWidth = GetLineFrameWidth();
            child->SetSize(lineFrameWidth, child->GetRect().Height());

            for(int i=0; i<columnCount; ++ i)
            {
                GetSubFrameId(i, frameId);
                NFrameBase* subFrame = child->GetChildById(frameId, false);
                if(subFrame == NULL)
                    break;
                subFrame->SetSize(header_->GetItemWidth(i), subFrame->GetRect().Height());
            }
            return true;
        }

        bool NList::OnHeaderItemWidthChanged(Base::NBaseObj*, NEventData* eventData)
        {
            NHeader::ItemWidthChangeEventData* data = static_cast<NHeader::ItemWidthChangeEventData*>(eventData);

            int offsetWidth = data->itemWidth - data->itemOldWidth;

            Base::NString frameId;
            NFrame* innerFrame = layout_->GetChildById(_NUI_INNER_FRAME_ID_, false);
            int lineCount = innerFrame->GetChildCount();
            int columnCount = header_->GetItemCount();

            int lineFrameWidth = GetLineFrameWidth();

            layout_->SetLayoutable(false);
            for(int i=0; i<lineCount; ++ i)
            {
                NFrame* lineFrame = innerFrame->GetChildAtIndex(static_cast<size_t>(i));
                lineFrame->SetSize(lineFrameWidth, lineFrame->GetRect().Height());

                GetSubFrameId(data->itemIndex, frameId);
                NFrame* subFrame = lineFrame->GetChildById(frameId, false);
                subFrame->SetSize(data->itemWidth, subFrame->GetRect().Height());

                for(int j=data->itemIndex+1; j<columnCount; ++ j)
                {
                    GetSubFrameId(j, frameId);
                    NFrame* subFrame = lineFrame->GetChildById(frameId, false);
                    const Base::NRect& frameRect = subFrame->GetRect();
                    subFrame->SetPos(frameRect.Left + offsetWidth, frameRect.Top);
                }
            }
            layout_->SetLayoutable(true);

            return true;
        }

        bool NList::OnHorzScrollPosChanged(Base::NBaseObj* baseObj, NEventData* eventData)
        {
            NScroll::ScrollEventData* data = static_cast<NScroll::ScrollEventData*>(eventData);
            NScroll* horzScroll = dynamic_cast<NScroll*>(baseObj);
            if(horzScroll && horzScroll->IsVisible())
                header_->SetPos(-data->scrollPos, 0);
            else
                header_->SetPos(0, 0);
            return true;
        }

        bool NList::OnHeaderSort(Base::NBaseObj*, NEventData* eventData)
        {
            NHeader::SortEventData* headerEventData = static_cast<NHeader::SortEventData*>(eventData);
            SortEventData listEventData;
            listEventData.itemIndex = headerEventData->itemIndex;
            listEventData.sortAsc = headerEventData->sortAsc;
            SortEvent.Invoke(this, &listEventData);
            return true;
        }

        void NList::ResetLineFrameWidth()
        {
            layout_->SetLayoutable(false);
            layout_->GetChildById(_NUI_INNER_FRAME_ID_, false)->EnumChilds(MakeDelegate(this, &NList::OnAdjustLinewWidths), 0);
            layout_->SetLayoutable(true);
        }

        int NList::GetLineFrameWidth() const
        {
            int lineFrameWidth = header_->GetRect().Width();
            NScroll* vertScroll = dynamic_cast<NScroll*>(layout_->GetChildById(_NUI_VERT_SCROLL_ID_, false));
            if(vertScroll && vertScroll->IsVisible())
                lineFrameWidth -= vertScroll->GetRect().Width();
            return lineFrameWidth;
        }

    }
}
