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
            NHeaderItem item;
            item.data_ = 0;
            item.flags_ = HeaderFlagNone;
            item.text_ = NUiBus::Instance().GetResourceLoader()->CreateText(text, MemToolParam);
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
            rcArrow.SetPos(0, rect.Left + (rect.Height() - sizeArrow.Height) / 2);
            rcArrow.SetSize(sizeArrow.Width, sizeArrow.Height);

            for(int i=0; i<count; ++ i)
            {
                const NHeaderItem& item = listItems_.GetAt(i);

                if(rcItem.Width() >= frameRect_.Right)
                    break;

                // bkg
                int index = 0;
                if(headerDragging_)
                    index = 0;
                else if(hoverItemIndex_ == i)
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

                // text
                if(item.text_ != NULL)
                {
                    rcText.Left = rcItem.Left + 1;
                    rcText.Right = rcArrow.Left - 1;
                    render->DrawText(item.text_, NULL, rcText);
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
            return size;
        }

        void NHeader::OnMouseDown(int x, int y)
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
                int itemIndex = RefreshCurrentItem(point.X, point.Y, inDragArea);
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
                    if((itemFlags & HeaderFlagNoSort) == HeaderFlagNoSort)
                        ;
                    else if((itemFlags & HeaderFlagSortAsc) == HeaderFlagSortAsc)
                        itemFlags = (itemFlags & (~HeaderFlagSortAsc)) | HeaderFlagSortDesc;
                    else // asc is default sort
                        itemFlags = (itemFlags & (~HeaderFlagSortDesc)) | HeaderFlagSortAsc;
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

        void NHeader::OnMouseMove(int x, int y)
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
                    listItems_[hoverItemIndex_].width_ = width;
                    AutoSize();
                    Invalidate();
                }
            }
            else
            {
                bool inDragArea = false;
                int newHoverItem = RefreshCurrentItem(x, y, inDragArea);
                static int flag = 0;
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
                RefreshCurrentItem(point.X, point.Y, dragging);
            }
            if(dragging)
                return GetCursorByType(NCursor::CursorLeftRight);
            return NULL;
        }

        void NHeader::RequireDraws()
        {
            NResourceLoader* loader = NUiBus::Instance().GetResourceLoader();
            headerDraw_ = loader->LoadImage(_T("@skin:common\\header.png"));
            arrowDraw_ = loader->LoadImage(_T("@skin:common\\headerArrow.png"));
            AutoSize();
        }

        int NHeader::RefreshCurrentItem(int x, int y, bool& inDragArea) const
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
                        inDragArea = i - 1 >= 0;
                        return i - 1;
                    }
                    else if(x >= rcItem.Right - dragMargin)
                    {
                        inDragArea = true;
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
