#include "stdafx.h"
#include "../NHeader.h"

#undef max
#undef min

namespace nui
{
    namespace Ui
    {
        IMPLEMENT_REFLECTION(NHeader);

        NHeader::NHeader()
        {
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagCanHover, true);
            Util::Misc::CheckFlag(frameFlags_, NFrame::FlagAutoSize, true);

            hoverItemIndex_ = -1;
            headerDragging_ = false;

            RequireDraws();
        }

        NHeader::~NHeader()
        {
            listItems_.Clear();
        }

        int NHeader::AddItem(int index, LPCTSTR text, int width)
        {
            return AddItem(index, text, width, HeaderFlagNone);
        }

        int NHeader::AddItem(int index, LPCTSTR text, int width, UINT flags)
        {
            NHeaderItem item;
            item.data_ = 0;
            item.flags_ = flags;
            item.text_ = text;
            item.textAttr_ = NUiBus::Instance().GetResourceLoader()->CreateText(MemToolParam);
            item.width_ = width;
            bool isAppend = index < 0 || index > listItems_.Count();
            if(isAppend)
                listItems_.AddItem(item);
            else
                listItems_.InsertAt(index, item);

            AutoSize();
            Invalidate();
            return isAppend ? listItems_.Count() - 1 : index;
        }

        void NHeader::RemoveItem(int index)
        {
            if(index >= 0 && index < listItems_.Count())
                listItems_.RemoveAt(index);

            AutoSize();
            Invalidate();
        }

        int NHeader::GetItemCount() const
        {
            return listItems_.Count();
        }

        int NHeader::GetItemWidth(int index) const
        {
            if(index >= 0 && index < listItems_.Count())
                return listItems_.GetAt(index).width_;
            return 0;
        }

        int NHeader::GetTotalWidth() const
        {
            int totalWidth = 0;
            int count = listItems_.Count();
            for(int i=0; i<count; ++ i)
            {
                const NHeaderItem& item = listItems_.GetAt(i);
                totalWidth += item.width_;
            }
            return totalWidth;
        }

        void NHeader::ClearAllItems()
        {
            listItems_.Clear();

            AutoSize();
            Invalidate();
        }

        void NHeader::SetItemData(int index, DWORD data)
        {
            if(index >= 0 && index < listItems_.Count())
                listItems_[index].data_ = data;
        }

        DWORD NHeader::GetItemData(int index) const
        {
            if(index >= 0 && index < listItems_.Count())
                return listItems_[index].data_;
            return 0;
        }

        void NHeader::OnCreate()
        {
            __super::OnCreate();

#ifndef _NO_NUI_PARSER_
            NAssertTempDisable();
            ApplyStyle(_T("@sys_default_style:header"));
#endif  // _NO_NUI_PARSER_
        }

        void NHeader::DrawBkg(NRender* render, const Base::NRect& rect) const
        {
            __super::DrawBkg(render, rect);

            headerDraw_->Draw(render, 0, 0, rect);
        }

        void NHeader::DrawContent(NRender* render, const Base::NRect& rect) const
        {
            int count = listItems_.Count();
            Base::NRect rcItem(rect);
            rcItem.Left = rcItem.Right = rect.Left;

            Base::NRect rcText(rect);
            Base::NSize sizeArrow = arrowDraw_->GetPreferSize();
            Base::NRect rcArrow;
            if(rect.Height() - 2 < sizeArrow.Height)
                sizeArrow.Height = rect.Height() - 2;
            rcArrow.SetPos(rect.Left, rect.Top + (rect.Height() - sizeArrow.Height) / 2);
            rcArrow.SetSize(sizeArrow.Width, sizeArrow.Height);

            for(int i=0; i<count; ++ i)
            {
                const NHeaderItem& item = listItems_.GetAt(i);

                int index = 0;
                if(!headerDragging_ && hoverItemIndex_ == i)
                    index = IsInStatus(NFrame::StatusPressed) ? 2 : 1;
                rcItem.Right = rcItem.Left + item.width_;
                headerDraw_->Draw(render, 0, index, rcItem);

                // arrow
                rcArrow.Left = rcItem.Right - sizeArrow.Width - 2;
                rcArrow.Right = rcArrow.Left + sizeArrow.Width;
                if(item.flags_ & HeaderFlagSortAsc)
                    arrowDraw_->Draw(render, 0, 0, rcArrow);
                else if(item.flags_ & HeaderFlagSortDesc)
                    arrowDraw_->Draw(render, 0, 1, rcArrow);
                else
                    rcArrow.Left = rcArrow.Right;

                // text
                if(!item.text_.IsEmpty())
                {
                    rcText.Left = rcItem.Left + 1;
                    rcText.Right = rcArrow.Left - 1;
                    render->DrawText(item.text_, item.textAttr_, NULL, rcText);
                }

                rcItem.Left = rcItem.Right;
            }
            __super::DrawContent(render, rect);
        }

        Base::NSize NHeader::GetAutoSize() const
        {
            Base::NSize size = __super::GetAutoSize();
            int totalWidth = 0;
            int count = listItems_.Count();
            for(int i=0; i<count; ++ i)
            {
                totalWidth += listItems_[i].width_;
            }
            Base::NSize drawSize = headerDraw_->GetPreferSize();
            size.Width = std::max(size.Width, totalWidth);
            size.Height = std::max(size.Height, drawSize.Height);
            return __super::GetAutoSize(size);
        }

        void NHeader::OnMouseDown(short x, short y)
        {
            bool inDragArea = false;
            int itemIndex = RefreshCurrentItem(x, y, inDragArea);
            if(itemIndex != -1)
            {
                hoverItemIndex_ = itemIndex;
                headerDragging_ = inDragArea;
            }
            __super::OnMouseDown(x, y);
        }

        void NHeader::OnMouseUp()
        {
            NAssertError(!headerDragging_ || hoverItemIndex_ >= 0, _T("wrong value in NHeader::OnMouseUp"));
            if(!headerDragging_ && hoverItemIndex_ != -1)
            {
                Base::NPoint point = Util::Shell::GetCurrentPos();
                Base::NRect rcWnd;
                window_->GetRect(rcWnd);
                point.X -= rcWnd.Left;
                point.Y -= rcWnd.Top;

                bool inDragArea = false;
                int itemIndex = RefreshCurrentItem(static_cast<short>(point.X), static_cast<short>(point.Y), inDragArea);
                if(itemIndex == hoverItemIndex_)
                {
                    int count = listItems_.Count();
                    for(int i=0; i<count; ++ i)
                    {
                        if(i == itemIndex)
                            continue;
                        UINT& flags = listItems_[i].flags_;
                        flags = flags & (~HeaderFlagSortAsc) & (~HeaderFlagSortDesc);
                    }
                    UINT& itemFlags = listItems_.GetAt(hoverItemIndex_).flags_;
                    if((itemFlags & HeaderFlagNoSort) != HeaderFlagNoSort)
                    {
                        if((itemFlags & HeaderFlagSortAsc) == HeaderFlagSortAsc)
                            itemFlags = (itemFlags & (~HeaderFlagSortAsc)) | HeaderFlagSortDesc;
                        else // asc is default sort
                            itemFlags = (itemFlags & (~HeaderFlagSortDesc)) | HeaderFlagSortAsc;

                        SortEventData data;
                        data.itemIndex = hoverItemIndex_;
                        data.sortAsc = (itemFlags & HeaderFlagSortAsc);
                        SortEvent.Invoke(this, &data);
                    }
                }
            }

            headerDragging_ = false;
            __super::OnMouseUp();
        }

        void NHeader::OnMouseLeave()
        {
            if(!headerDragging_)
                hoverItemIndex_ = -1;
            __super::OnMouseLeave();
        }

        void NHeader::OnMouseMove(short x, short y)
        {
            if(headerDragging_)
            {
                Base::NRect rcHeader = GetRootRect();
                const int minItemWidth = 10;
                int width = x - rcHeader.Left;
                for(int i=0; i<hoverItemIndex_; ++ i)
                    width -= listItems_[i].width_;

                if(width >= minItemWidth && width != listItems_[hoverItemIndex_].width_)
                {
                    int oldWidth = listItems_[hoverItemIndex_].width_;
                    listItems_[hoverItemIndex_].width_ = width;

                    ItemWidthChangeEventData eventData;
                    eventData.itemOldWidth = oldWidth;
                    eventData.itemIndex = hoverItemIndex_;
                    eventData.itemWidth = width;
                    ItemWidthChangeEvent.Invoke(this, &eventData);

                    AutoSize();
                    Invalidate();
                }
            }
            else
            {
                bool inDragArea = false;
                int newHoverItem = RefreshCurrentItem(x, y, inDragArea);
                if(inDragArea)
                    newHoverItem = -1;
                if(hoverItemIndex_ != newHoverItem)
                {
                    hoverItemIndex_ = newHoverItem;
                    Invalidate();
                }
            }

            __super::OnMouseMove(x, y);
        }

        NCursor* NHeader::GetCursor() const
        {
            bool dragging = headerDragging_;
            if(!dragging)
            {
                Base::NPoint point = Util::Shell::GetCurrentPos(window_);
                RefreshCurrentItem(static_cast<short>(point.X), static_cast<short>(point.Y), dragging);
            }
            if(dragging)
            {
                return GetCursorByType(NCursor::CursorLeftRight);
            }
            return NULL;
        }

        void NHeader::RequireDraws()
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            headerDraw_ = loader->LoadImage(_T("@skin:common\\header.png"));
            arrowDraw_ = loader->LoadImage(_T("@skin:common\\headerArrow.png"));
            AutoSize();
        }

        int NHeader::RefreshCurrentItem(short x, short y, bool& inDragArea) const
        {
            Base::NRect rcItem = GetRootRect();
            if(!rcItem.Contains(x, y))
                return -1;

            const int dragMargin = 2;
            int count = listItems_.Count();
            for(int i=0; i<count; ++ i)
            {
                const NHeaderItem& item = listItems_[i];
                rcItem.Right = rcItem.Left + item.width_;
                if(rcItem.Contains(x, y))
                {
                    if(x < rcItem.Left + dragMargin)
                    {
                        inDragArea = i - 1 >= 0 && ((item.flags_ & HeaderFlagFixedWidth) != HeaderFlagFixedWidth);
                        return i - 1;
                    }
                    else if(x >= rcItem.Right - dragMargin)
                    {
                        inDragArea = ((item.flags_ & HeaderFlagFixedWidth) != HeaderFlagFixedWidth);;
                        return i;
                    }
                    else
                    {
                        inDragArea = false;
                        return i;
                    }
                }
                rcItem.Left = rcItem.Right;
            }
            return -1;
        }
    }
}
